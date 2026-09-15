'use strict';

const fs = require('fs');
const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { PQController } = require('../parsing/parser');
const { Collection } = require('../core/collection');
const { RecordHolder, RecType } = require('./recordHolder');
const Container = require('./container');
const DataSource = require('./dataSource');
const MonitorSettings = require('./monitorSettings');
const Observation = require('./observation');
const { xmlGetElement } = require('./utilities');

// Record header size: 64 bytes
const RECORD_HEADER_SIZE = 64;

// Compression constants
const ID_COMP_STYLE_RECORDLEVEL = 2;

/**
 * For reading and writing PQDIF files - ported from File.cs, Read.cs, Save.cs.
 */
class PqFile {
  constructor() {
    /** @type {string} File name */
    this.fileName = '';

    /** @type {boolean} True if file has been read */
    this.fileRead = false;

    /** @type {boolean} True if file was read successfully */
    this.fileOK = false;

    /** @type {Logger} Application logger */
    this.loggerApplication = new Logger();

    /** @type {Logger} Compliance logger */
    this.loggerCompliance = new Logger();

    /** @type {number|null} Max series values for XML export */
    this.maxSeriesValues = null;

    /** @type {RecordHolder[]} List of record holders */
    this.recordHolders = [];
  }

  /**
   * Read a PQDIF file.
   * @param {string} fileName - Path to the PQDIF file
   * @returns {boolean} True if file was read OK
   */
  read(fileName) {
    this.fileName = fileName;
    return this._read();
  }

  /**
   * Internal read method.
   * @returns {boolean}
   * @private
   */
  _read() {
    try {
      // Clear logs
      this.loggerApplication.logsClear();
      this.loggerCompliance.logsClear();

      this.fileOK = false;

      // Read the entire file into memory
      const fileBuf = fs.readFileSync(this.fileName);
      const fileSize = fileBuf.length;

      if (fileSize < RECORD_HEADER_SIZE) {
        this.loggerApplication.log('File too small to contain a PQDIF record header.', LogLevels.Error);
        return false;
      }

      // Read first record header to get compression info
      let compressionStyle = 0;
      let compressionAlgorithm = 0;

      const sig = fileBuf.subarray(0, 16);
      if (!guidEquals(sig, tagGuids.guidRecordSignaturePQDIF)) {
        this.loggerApplication.log('Invalid PQDIF file signature.', LogLevels.Error);
        return false;
      }

      const sizeHeader = fileBuf.readInt32LE(32);
      const sizeData = fileBuf.readInt32LE(36);
      let posNextRecord = fileBuf.readInt32LE(40);

      // Parse first record body to find compression tags
      const firstBodyStart = RECORD_HEADER_SIZE;
      const firstBodyEnd = firstBodyStart + sizeData;
      if (firstBodyEnd <= fileSize) {
        const firstBody = fileBuf.subarray(firstBodyStart, firstBodyEnd);
        this._parseContainerCompression(firstBody, (style, alg) => {
          compressionStyle = style;
          compressionAlgorithm = alg;
        });
      }

      // Set up compression processor for record bodies
      let bodyProcessor = null;
      if (compressionStyle === ID_COMP_STYLE_RECORDLEVEL) {
        try {
          const { ZlibProcessor } = require('../io/streamProcessor');
          bodyProcessor = new ZlibProcessor();
        } catch (e) {
          this.loggerApplication.log('Failed to create ZlibProcessor: ' + e.message, LogLevels.Error);
        }
      }

      // Reset tracking
      this.recordHolders = [];
      let containerFound = false;
      let recDataSourceFound = false;
      let recObservationFound = false;
      let lastRecDataSource = null;
      let lastRecMonitorSettings = null;

      // Iterate through all records
      let pos = 0;
      let recIndex = 0;

      while (pos < fileSize) {
        if (pos + RECORD_HEADER_SIZE > fileSize) break;

        // Read header
        const tagRecordType = Buffer.from(fileBuf.subarray(pos + 16, pos + 32));
        const sizeHdr = fileBuf.readInt32LE(pos + 32);
        const sizeRec = fileBuf.readInt32LE(pos + 36);
        posNextRecord = fileBuf.readInt32LE(pos + 40);

        const logPrefix = 'Record ' + recIndex;

        // Read body
        const bodyStart = pos + sizeHdr;
        const bodyEnd = bodyStart + sizeRec;

        if (bodyEnd > fileSize) {
          this.loggerApplication.log(logPrefix + ': Record body extends beyond file.', LogLevels.Error);
          break;
        }

        let bodyBuf = fileBuf.subarray(bodyStart, bodyEnd);

        // Decompress if needed (container record is never compressed)
        if (bodyProcessor && sizeRec > 0 && recIndex > 0) {
          try {
            const { ChunkStreamIO } = require('../io/chunkStreamIO');
            const chunkStream = new ChunkStreamIO();
            chunkStream.setInput(bodyBuf, sizeRec);
            chunkStream.connectProcessor(bodyProcessor);
            const result = chunkStream.readBlock(sizeRec);
            if (result && result.data) {
              bodyBuf = result.data.subarray(0, result.size);
            }
          } catch (e) {
            this.loggerApplication.log(logPrefix + ': Decompression error: ' + e.message, LogLevels.Error);
          }
        }

        // Parse body into Element tree
        let mainCollection = null;
        if (bodyBuf.length > 0) {
          try {
            mainCollection = new Collection();
            const controller = new PQController(mainCollection);
            controller.parseRecord(bodyBuf, 0, bodyBuf.length);
          } catch (e) {
            this.loggerApplication.log(logPrefix + ': Parse error: ' + e.message, LogLevels.Error);
          }
        }

        // Identify and create record wrapper
        if (guidEquals(tagRecordType, tagGuids.tagContainer)) {
          this.loggerApplication.log(logPrefix + ': Container Found: ' + sizeRec + ' bytes', LogLevels.Info);
          containerFound = true;

          let recordOK = true;
          let container = null;
          try {
            container = Container.fromCollection(mainCollection);
          } catch (e) {
            this.loggerApplication.log(logPrefix + ': Container parse error: ' + e.message, LogLevels.Error);
            recordOK = false;
          }

          if (recordOK && container) {
            this.recordHolders.push(new RecordHolder(recIndex, RecType.Container, container));
          }

          if (recIndex !== 0 && this.recordHolders.length > 1) {
            this.loggerCompliance.log(logPrefix + ': tagContainer should be the first top-level record of a PQDIF file.', LogLevels.Error);
          }
        } else if (guidEquals(tagRecordType, tagGuids.tagRecDataSource)) {
          this.loggerApplication.log(logPrefix + ': Data Source Record Found: ' + sizeRec + ' bytes', LogLevels.Info);
          recDataSourceFound = true;

          let recordOK = true;
          let ds = null;
          try {
            ds = DataSource.fromCollection(mainCollection);
          } catch (e) {
            this.loggerApplication.log(logPrefix + ': DataSource parse error: ' + e.message, LogLevels.Error);
            recordOK = false;
          }

          if (recordOK && ds) {
            const holder = new RecordHolder(recIndex, RecType.RecDataSource, ds);
            this.recordHolders.push(holder);
            lastRecDataSource = ds;
          }
        } else if (guidEquals(tagRecordType, tagGuids.tagRecMonitorSettings)) {
          this.loggerApplication.log(logPrefix + ': Monitor Settings Record Found: ' + sizeRec + ' bytes', LogLevels.Info);
          recDataSourceFound = true;

          let recordOK = true;
          let ms = null;
          try {
            ms = MonitorSettings.fromCollection(mainCollection);
          } catch (e) {
            this.loggerApplication.log(logPrefix + ': MonitorSettings parse error: ' + e.message, LogLevels.Error);
            recordOK = false;
          }

          if (recordOK && ms) {
            const holder = new RecordHolder(recIndex, RecType.RecMonitorSettings, ms);
            holder.dataSource = lastRecDataSource;
            this.recordHolders.push(holder);
            lastRecMonitorSettings = ms;
          }
        } else if (guidEquals(tagRecordType, tagGuids.tagRecObservation)) {
          this.loggerApplication.log(logPrefix + ': Observation Record Found: ' + Math.floor(sizeRec / 1024) + ' kbytes', LogLevels.Info);
          recObservationFound = true;

          let recordOK = true;
          let obs = null;
          try {
            obs = Observation.fromCollection(mainCollection);
          } catch (e) {
            this.loggerApplication.log(logPrefix + ': Observation parse error: ' + e.message, LogLevels.Error);
            recordOK = false;
          }

          if (recordOK && obs) {
            const holder = new RecordHolder(recIndex, RecType.RecObservation, obs);
            holder.dataSource = lastRecDataSource;
            holder.monitorSettings = lastRecMonitorSettings;
            this.recordHolders.push(holder);
          }
        } else if (guidEquals(tagRecordType, tagGuids.tagBlank)) {
          this.loggerApplication.log(logPrefix + ': Blank Record Found: ' + sizeRec + ' bytes', LogLevels.Info);
        } else {
          this.loggerCompliance.log(logPrefix + ': Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tagRecordType) || tagRecordType.toString('hex')), LogLevels.Info);
        }

        // Move to next record
        if (posNextRecord <= 0 || posNextRecord >= fileSize) break;
        pos = posNextRecord;
        recIndex++;
      }

      // Check for required records
      if (!containerFound && recIndex > 1) {
        this.loggerCompliance.log('Required Container Missing.', LogLevels.Error);
      }
      if (!recDataSourceFound && recIndex > 1) {
        this.loggerCompliance.log('Required Data Source Record Missing.', LogLevels.Error);
      }
      if (!recObservationFound && recIndex > 1) {
        this.loggerCompliance.log('Required Observation Record Missing.', LogLevels.Error);
      }

      // Validate channel instance counts
      this._validateChannelInstanceCount();

      // Aggregate logs from records
      this._addLogsFromRecords();

      this.fileOK = true;
      this.fileRead = true;

    } catch (ex) {
      this.loggerApplication.log('Error: ' + ex.message, LogLevels.Error);
    }

    return this.fileOK;
  }

  /**
   * Save records to a new PQDIF file.
   * @param {string} newFileName - Path for the new file
   * @param {boolean} [removePI=false] - Remove personal information
   * @param {string} [newNameDS=''] - Optional replacement data source name
   * @returns {boolean} True on success
   */
  save(newFileName, removePI, newNameDS) {
    removePI = removePI || false;
    newNameDS = newNameDS || '';

    let savedOK = true;

    try {
      const FlatFileController = require('../persistence/flatFileController');
      const factory = require('../core/factory');
      const {
        ID_COMP_STYLE_NONE, ID_COMP_ALG_NONE,
      } = require('../constants/compressionIds');

      const controller = new FlatFileController();
      controller.setFileName(newFileName);
      controller.setCompressionStyle(ID_COMP_STYLE_NONE);
      controller.setCompressionAlgorithm(ID_COMP_ALG_NONE);

      let nameDS = '';
      let recIndex = 0;

      for (const rh of this.recordHolders) {
        if (rh.recType === RecType.Container) {
          this.loggerApplication.log(recIndex + ': Adding Container', LogLevels.Info);
          const container = rh.record;

          controller.createContainerRecord(
            newFileName,
            container.versionInfo,
            container.creation,
            container.versionInfo[0] || 0,
            container.versionInfo[1] || 0,
            container.versionInfo[2] || 0,
            container.versionInfo[3] || 0
          );

          if (!removePI) {
            controller.createContainerRecordWithMeta(
              container.language, container.title, container.subject,
              container.author, container.keywords, container.comments,
              container.lastSavedBy, container.application, container.security,
              container.owner, container.copyright, container.trademarks, container.notes
            );
          }
        }

        if (rh.recType === RecType.RecDataSource) {
          this.loggerApplication.log(recIndex + ': Adding Data Source Record', LogLevels.Info);
          const ds = rh.record;

          nameDS = ds.nameDS;
          if (newNameDS.length > 0) nameDS = newNameDS;
          else if (removePI) nameDS = '';

          controller.createDataSourceRecord(
            recIndex,
            ds.dataSourceTypeID,
            ds.vendorID,
            ds.equipmentID,
            removePI ? '' : ds.serialNumberDS,
            removePI ? '' : ds.versionDS,
            nameDS,
            removePI ? '' : ds.ownerDS,
            removePI ? '' : ds.locationDS,
            removePI ? '' : ds.timeZoneDS
          );
        }

        if (rh.recType === RecType.RecMonitorSettings) {
          this.loggerApplication.log(recIndex + ': Adding Monitor Settings Record', LogLevels.Info);
          const ms = rh.record;

          controller.createMonitorSettingsRecord(
            recIndex,
            ms.effective,
            ms.timeInstalled,
            ms.timeRemoved,
            ms.useCalibration,
            ms.useTransducer
          );
        }

        if (rh.recType === RecType.RecObservation) {
          this.loggerApplication.log(recIndex + ': Adding Observation Record', LogLevels.Info);
          const obs = rh.record;

          let obsName = obs.observationName;
          if (removePI) obsName = nameDS + ' ' + obs.timeStart.toISOString();

          controller.createObservationRecord(
            recIndex,
            obsName,
            obs.timeCreate,
            obs.timeStart,
            obs.triggerMethodID,
            obs.timeTriggered,
            obs.channelTriggerIdx ? obs.channelTriggerIdx.length : 0,
            obs.channelTriggerIdx
          );
        }

        recIndex++;
      }

      // Write the file
      const writeOK = controller.writeNew();
      if (!writeOK) {
        this.loggerApplication.log('Error writing PQDIF file.', LogLevels.Error);
        savedOK = false;
      }

    } catch (ex) {
      this.loggerApplication.log('Error: ' + ex.message, LogLevels.Error);
      savedOK = false;
    }

    return savedOK;
  }

  /**
   * Get sorted list of data source names.
   * @returns {string[]}
   */
  getDataSourceNames() {
    const names = [];
    for (const rh of this.recordHolders) {
      if (rh.recType === RecType.RecDataSource) {
        const ds = rh.record;
        if (!names.includes(ds.nameDS)) names.push(ds.nameDS);
      }
    }
    names.sort();
    return names;
  }

  /**
   * Get earliest observation time start.
   * @returns {Date|null}
   */
  getTimeStartMin() {
    let found = false;
    let min = new Date(Date.MAX_VALUE);
    for (const rh of this.recordHolders) {
      if (rh.recType === RecType.RecObservation) {
        found = true;
        const obs = rh.record;
        if (obs.timeStart < min) min = obs.timeStart;
      }
    }
    return found ? min : null;
  }

  /**
   * Get latest observation time start.
   * @returns {Date|null}
   */
  getTimeStartMax() {
    let found = false;
    let max = new Date(Date.MIN_VALUE);
    for (const rh of this.recordHolders) {
      if (rh.recType === RecType.RecObservation) {
        found = true;
        const obs = rh.record;
        if (obs.timeStart > max) max = obs.timeStart;
      }
    }
    return found ? max : null;
  }

  /**
   * Export entire file as XML.
   * @param {boolean} [applicationLog=false]
   * @param {boolean} [complianceLog=false]
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<?xml version="1.0"?>';
    xml += '<records>';

    for (const rh of this.recordHolders) {
      if (rh.recType === RecType.Container) {
        xml += rh.record.getXML(applicationLog, complianceLog);
      } else if (rh.recType === RecType.RecDataSource) {
        xml += rh.record.getXML(applicationLog, complianceLog);
      } else if (rh.recType === RecType.RecMonitorSettings) {
        xml += rh.record.getXML(applicationLog, complianceLog);
      } else if (rh.recType === RecType.RecObservation) {
        xml += rh.record.getXML(applicationLog, complianceLog, this.maxSeriesValues);
      }
    }

    xml += '</records>';
    return xml;
  }

  /**
   * Export entire file as JSON.
   * @param {number|null} [maxSeriesValues=null]
   * @returns {Object}
   */
  getJSON(maxSeriesValues) {
    const effectiveMax = maxSeriesValues !== undefined && maxSeriesValues !== null
      ? maxSeriesValues
      : this.maxSeriesValues;

    return {
      fileName: this.fileName,
      records: this.recordHolders.map(rh => {
        const base = rh.getJSON();
        if (rh.recType === RecType.RecObservation && base.record && effectiveMax !== null) {
          base.record = rh.record.getJSON(effectiveMax);
        }
        return base;
      })
    };
  }

  /**
   * Export observation data as CSV.
   *
   * For each observation, produces a section with:
   *   - A header row with column names
   *   - Data rows with series values
   *
   * Columns: ObservationName, TimeStart, ChannelName, SeriesIndex, Value0, Value1, ...
   *
   * @param {number|null} [maxValuesPerSeries=null] - Max values per series (truncates long waveforms)
   * @returns {string} CSV content
   */
  toCSV(maxValuesPerSeries) {
    maxValuesPerSeries = maxValuesPerSeries || null;
    const rows = [];

    // Header
    rows.push([
      'ObservationName',
      'TimeCreate',
      'TimeStart',
      'ChannelDefnIdx',
      'ChannelName',
      'PhaseID',
      'QuantityType',
      'SeriesIndex',
      'ValueType',
      'StorageMethod',
      'Values'
    ].join(','));

    for (const rh of this.recordHolders) {
      if (rh.recType !== RecType.RecObservation) continue;

      const obs = rh.record;
      const ds = rh.dataSource;
      if (!ds) continue;

      for (const ci of obs.channelInstances) {
        const chDefnIdx = ci.channelDefnIdx;
        const chDefn = (chDefnIdx < ds.channelDefns.length) ? ds.channelDefns[chDefnIdx] : null;
        const channelName = chDefn ? chDefn.channelName : '';
        const phaseID = chDefn ? chDefn.phaseID : '';
        const quantityType = chDefn ? chDefn.quantityTypeID : '';

        for (let si = 0; si < ci.seriesInstances.length; si++) {
          const seriesInst = ci.seriesInstances[si];
          const seriesDefn = (chDefn && si < chDefn.seriesDefns.length) ? chDefn.seriesDefns[si] : null;
          const valueType = seriesDefn ? seriesDefn.valueTypeID : '';
          const storageMethod = seriesDefn ? seriesDefn.storageMethodID : '';

          // Get values as array
          let values = [];
          if (seriesInst.seriesValues) {
            values = Array.from(seriesInst.seriesValues);
          }

          // Truncate if needed
          if (maxValuesPerSeries && values.length > maxValuesPerSeries) {
            values = values.slice(0, maxValuesPerSeries);
          }

          // Format values
          const valuesStr = values.map(v => typeof v === 'number' ? v : String(v)).join(';');

          rows.push([
            _csvEscape(obs.observationName || ''),
            obs.timeCreate ? obs.timeCreate.toISOString() : '',
            obs.timeStart ? obs.timeStart.toISOString() : '',
            chDefnIdx,
            _csvEscape(channelName),
            phaseID,
            _csvGuid(quantityType),
            si,
            _csvGuid(valueType),
            storageMethod,
            '"' + valuesStr + '"'
          ].join(','));
        }
      }
    }

    return rows.join('\n');
  }

  /**
   * Export a summary of all records as CSV.
   * @returns {string} CSV content
   */
  toSummaryCSV() {
    const rows = [];

    // Header
    rows.push('RecordIndex,RecordType,Name,TimeStart,ChannelCount,SeriesCount,ValueCount');

    for (const rh of this.recordHolders) {
      let name = '';
      let timeStart = '';
      let channelCount = 0;
      let seriesCount = 0;
      let valueCount = 0;

      if (rh.recType === RecType.Container) {
        name = rh.record.fileName || '';
      } else if (rh.recType === RecType.RecDataSource) {
        name = rh.record.nameDS || '';
        channelCount = rh.record.channelDefns.length;
      } else if (rh.recType === RecType.RecMonitorSettings) {
        name = 'MonitorSettings';
      } else if (rh.recType === RecType.RecObservation) {
        name = rh.record.observationName || '';
        timeStart = rh.record.timeStart ? rh.record.timeStart.toISOString() : '';
        channelCount = rh.record.channelInstances.length;
        for (const ci of rh.record.channelInstances) {
          seriesCount += ci.seriesInstances.length;
          for (const si of ci.seriesInstances) {
            if (si.seriesValues) valueCount += si.seriesValues.length;
          }
        }
      }

      rows.push([
        rh.recIndex,
        RecType[rh.recType] || rh.recType,
        _csvEscape(name),
        timeStart,
        channelCount,
        seriesCount,
        valueCount
      ].join(','));
    }

    return rows.join('\n');
  }

  /**
   * Validate channel instance counts against channel definition counts.
   * @private
   */
  _validateChannelInstanceCount() {
    for (const rh of this.recordHolders) {
      if (rh.recType === RecType.RecObservation && rh.dataSource) {
        const obs = rh.record;
        const ds = rh.dataSource;

        for (const ci of obs.channelInstances) {
          const chDefnIdx = ci.channelDefnIdx;
          if (chDefnIdx < ds.channelDefns.length) {
            const chDefn = ds.channelDefns[chDefnIdx];
            if (ci.seriesInstances.length !== chDefn.seriesDefns.length) {
              ci.loggerCompliance.log(
                'The count of series instances, which is ' + ci.seriesInstances.length +
                ', should be equal to the count of series definitions in the related data source record. ' +
                'The count of series definitions is ' + chDefn.seriesDefns.length + '.',
                LogLevels.Error
              );
            }
          }
        }
      }
    }
  }

  /**
   * Aggregate logs from all records into the file-level loggers.
   * @private
   */
  _addLogsFromRecords() {
    let recIndex = 0;

    for (const rh of this.recordHolders) {
      if (rh.recType === RecType.Container) {
        this._copyLogs(rh.record.loggerApplication, rh.record.loggerCompliance, 'Container (RecIndex = ' + recIndex + '): ');
      } else if (rh.recType === RecType.RecDataSource) {
        const ds = rh.record;
        const prefix = 'Data Source (RecIndex = ' + recIndex + '): ';
        this._copyLogs(ds.loggerApplication, ds.loggerCompliance, prefix);

        for (let chIdx = 0; chIdx < ds.channelDefns.length; chIdx++) {
          const chDefn = ds.channelDefns[chIdx];
          const chPrefix = prefix + 'Channel Definition ' + chIdx + ': ';
          this._copyLogs(chDefn.loggerApplication, chDefn.loggerCompliance, chPrefix);

          for (let sdIdx = 0; sdIdx < chDefn.seriesDefns.length; sdIdx++) {
            const sd = chDefn.seriesDefns[sdIdx];
            const sdPrefix = chPrefix + 'Series Definition ' + sdIdx + ': ';
            this._copyLogs(sd.loggerApplication, sd.loggerCompliance, sdPrefix);
          }
        }
      } else if (rh.recType === RecType.RecMonitorSettings) {
        const ms = rh.record;
        const prefix = 'Monitor Settings Record (RecIndex = ' + recIndex + '): ';
        this._copyLogs(ms.loggerApplication, ms.loggerCompliance, prefix);

        for (let csIdx = 0; csIdx < ms.channelSettingsArray.length; csIdx++) {
          const cs = ms.channelSettingsArray[csIdx];
          const csPrefix = prefix + 'Channel Setting ' + csIdx + ': ';
          this._copyLogs(cs.loggerApplication, cs.loggerCompliance, csPrefix);
        }
      } else if (rh.recType === RecType.RecObservation) {
        const obs = rh.record;
        const prefix = 'Observation Record (RecIndex = ' + recIndex + '): ';
        this._copyLogs(obs.loggerApplication, obs.loggerCompliance, prefix);

        for (let ciIdx = 0; ciIdx < obs.channelInstances.length; ciIdx++) {
          const ci = obs.channelInstances[ciIdx];
          const ciPrefix = prefix + 'Channel Instance ' + ciIdx + ': ';
          this._copyLogs(ci.loggerApplication, ci.loggerCompliance, ciPrefix);

          for (let siIdx = 0; siIdx < ci.seriesInstances.length; siIdx++) {
            const si = ci.seriesInstances[siIdx];
            const siPrefix = ciPrefix + 'Series Instance ' + siIdx + ': ';
            this._copyLogs(si.loggerApplication, si.loggerCompliance, siPrefix);
          }
        }
      }

      recIndex++;
    }
  }

  /**
   * Deep copy logs from sub-loggers to file-level loggers.
   * @param {Logger} subApp
   * @param {Logger} subCompliance
   * @param {string} prefix
   * @private
   */
  _copyLogs(subApp, subCompliance, prefix) {
    if (subApp && subApp.getLogEventList) {
      for (const evt of subApp.getLogEventList()) {
        this.loggerApplication.log(evt.message ? prefix + evt.message : prefix, evt.level);
      }
    }
    if (subCompliance && subCompliance.getLogEventList) {
      for (const evt of subCompliance.getLogEventList()) {
        this.loggerCompliance.log(evt.message ? prefix + evt.message : prefix, evt.level);
      }
    }
  }

  /**
   * Parse compression info from a container body buffer.
   * @param {Buffer} bodyBuf
   * @param {function(number, number): void} setCompression
   * @private
   */
  _parseContainerCompression(bodyBuf, setCompression) {
    if (bodyBuf.length < 4) return;
    const count = bodyBuf.readInt32LE(0);
    if (count <= 0) return;

    let style = 0;
    let alg = 0;
    let foundStyle = false;
    let foundAlg = false;
    const elemBase = 4;

    for (let i = 0; i < count; i++) {
      const elemOffset = elemBase + i * 28;
      if (elemOffset + 28 > bodyBuf.length) break;

      const tag = bodyBuf.subarray(elemOffset, elemOffset + 16);

      if (guidEquals(tag, tagGuids.tagCompressionStyleID)) {
        const isEmbedded = bodyBuf.readUInt8(elemOffset + 18);
        if (isEmbedded) {
          style = bodyBuf.readUInt32LE(elemOffset + 20);
        } else {
          const linkOffset = bodyBuf.readInt32LE(elemOffset + 20);
          if (linkOffset > 0 && linkOffset + 4 <= bodyBuf.length) {
            style = bodyBuf.readInt32LE(linkOffset);
          }
        }
        foundStyle = true;
      }

      if (guidEquals(tag, tagGuids.tagCompressionAlgorithmID)) {
        const isEmbedded = bodyBuf.readUInt8(elemOffset + 18);
        if (isEmbedded) {
          alg = bodyBuf.readUInt32LE(elemOffset + 20);
        } else {
          const linkOffset = bodyBuf.readInt32LE(elemOffset + 20);
          if (linkOffset > 0 && linkOffset + 4 <= bodyBuf.length) {
            alg = bodyBuf.readInt32LE(linkOffset);
          }
        }
        foundAlg = true;
      }

      if (foundStyle && foundAlg) break;
    }

    if (foundStyle || foundAlg) {
      setCompression(style, alg);
    }
  }

  /**
   * Get min/max timestamps from observation series instances.
   * @param {RecordHolder} recordHolder
   * @returns {{ min: Date, max: Date }|null}
   */
  getRecObservationTimeStampMinMax(recordHolder) {
    if (recordHolder.recType !== RecType.RecObservation) return null;

    const obs = recordHolder.record;
    let tsMin = obs.timeStart;
    let tsMax = obs.timeStart;

    for (const ci of obs.channelInstances) {
      const chDefn = ci.getChannelDefn(recordHolder.dataSource);
      if (chDefn) {
        const mm = ci.getTimeStartMinMax(obs, chDefn);
        if (mm.min && mm.min < tsMin) tsMin = mm.min;
        if (mm.max && mm.max > tsMax) tsMax = mm.max;
      }
    }

    return { min: tsMin, max: tsMax };
  }

  /**
   * Get the nominal voltage for a given observation record.
   * @param {RecordHolder} recordHolder
   * @returns {number|null}
   */
  getNominalVoltage(recordHolder) {
    if (recordHolder.recType !== RecType.RecObservation || !recordHolder.dataSource) return null;

    const obs = recordHolder.record;
    const ds = recordHolder.dataSource;
    const Peak2Rms = 0.70710678118654757;
    const idQmVoltage = require('../constants/idIntegers').ID_QM_VOLTAGE;
    const idQcInstantaneous = require('../constants/idGuids').ID_QC_INSTANTANEOUS;
    const idQcRms = require('../constants/idGuids').ID_QC_RMS;
    const idValueTypeTime = require('../constants/idGuids').ID_SERIES_VALUE_TYPE_TIME;

    for (const ci of obs.channelInstances) {
      const chDefn = ci.getChannelDefn(ds);
      if (!chDefn) continue;

      if (chDefn.quantityMeasuredID === idQmVoltage &&
          chDefn.seriesDefns.length === ci.seriesInstances.length) {
        for (let si = 0; si < ci.seriesInstances.length; si++) {
          const sd = chDefn.seriesDefns[si];
          const siInst = ci.seriesInstances[si];
          if ((guidEquals(sd.quantityCharacteristicID, idQcInstantaneous) ||
               guidEquals(sd.quantityCharacteristicID, idQcRms)) &&
              (sd.valueTypeID && !guidEquals(sd.valueTypeID, idValueTypeTime))) {
            if (siInst.seriesBaseQuantity !== null) {
              if (guidEquals(sd.quantityCharacteristicID, idQcInstantaneous)) {
                return siInst.seriesBaseQuantity * Peak2Rms;
              } else {
                return siInst.seriesBaseQuantity;
              }
            }
          }
        }
      }
    }

    // Fall back to data source
    return ds.getNominalVoltage();
  }

  /**
   * Get the timestamp in Local Standard Time.
   * @param {RecordHolder} recordHolder
   * @param {Date} timestampUTC
   * @returns {Date}
   */
  getTimeStampLST(recordHolder, timestampUTC) {
    let utcToLST = 0;

    if (recordHolder.recType === RecType.RecDataSource) {
      const ds = recordHolder.record;
      if (ds.utcToLST !== null) utcToLST = ds.utcToLST;
    } else if (recordHolder.dataSource) {
      if (recordHolder.dataSource.utcToLST !== null) utcToLST = recordHolder.dataSource.utcToLST;
    }

    return new Date(timestampUTC.getTime() + utcToLST * 1000);
  }

  /**
   * Get the timestamp in Local Time (UTC + UTCtoLST + LSTtoLocal).
   * @param {RecordHolder} recordHolder
   * @param {Date} timestampUTC
   * @returns {Date}
   */
  getTimeStampLocal(recordHolder, timestampUTC) {
    let utcToLST = 0;
    let lsttoLocal = 0;

    if (recordHolder.recType === RecType.RecDataSource) {
      const ds = recordHolder.record;
      if (ds.utcToLST !== null) utcToLST = ds.utcToLST;
    } else if (recordHolder.recType === RecType.RecMonitorSettings) {
      const ms = recordHolder.record;
      if (ms.lsttoLocal !== null) lsttoLocal = ms.lsttoLocal;
      if (recordHolder.dataSource && recordHolder.dataSource.utcToLST !== null) {
        utcToLST = recordHolder.dataSource.utcToLST;
      }
    } else if (recordHolder.recType === RecType.RecObservation) {
      const obs = recordHolder.record;
      if (obs.lsttoLocal !== null) {
        lsttoLocal = obs.lsttoLocal;
      } else if (recordHolder.monitorSettings && recordHolder.monitorSettings.lsttoLocal !== null) {
        lsttoLocal = recordHolder.monitorSettings.lsttoLocal;
      }
      if (recordHolder.dataSource && recordHolder.dataSource.utcToLST !== null) {
        utcToLST = recordHolder.dataSource.utcToLST;
      }
    }

    return new Date(timestampUTC.getTime() + (utcToLST + lsttoLocal) * 1000);
  }
}

// CSV helper: escape a value for CSV (wrap in quotes if it contains commas/quotes/newlines)
function _csvEscape(val) {
  if (val === null || val === undefined) return '';
  const s = String(val);
  if (s.includes(',') || s.includes('"') || s.includes('\n')) {
    return '"' + s.replace(/"/g, '""') + '"';
  }
  return s;
}

// CSV helper: format a GUID buffer as a hex string, or return empty
function _csvGuid(guid) {
  if (!guid) return '';
  if (Buffer.isBuffer(guid)) return guid.toString('hex');
  return String(guid);
}

module.exports = PqFile;
