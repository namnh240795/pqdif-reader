'use strict';

const { Record } = require('./record');
const { Collection } = require('./collection');
const { PqdifInfo, guidEquals } = require('../info/info');
const {
  ID_PHYS_TYPE_UNS_INTEGER4, ID_PHYS_TYPE_REAL8, ID_PHYS_TYPE_REAL4,
  ID_PHYS_TYPE_TIMESTAMPPQDIF,
  SECONDS_PER_DAY,
  ID_SERIES_METHOD_VALUES, ID_SERIES_METHOD_INCREMENT, ID_SERIES_METHOD_SCALED,
  ID_TRIGGER_METH_CHANNEL,
} = require('../constants/physicalTypes');
const {
  ID_ELEMENT_TYPE_COLLECTION, ID_ELEMENT_TYPE_SCALAR, ID_ELEMENT_TYPE_VECTOR,
} = require('../constants/elementTypes');
const {
  tagTimeStart, tagTimeCreate, tagObservationName,
  tagTriggerMethodID, tagTimeTriggered, tagChannelTriggerIdx,
  tagChannelInstances, tagOneChannelInst, tagChannelDefnIdx,
  tagSeriesInstances, tagOneSeriesInstance,
  tagSeriesBaseQuantity, tagSeriesScale, tagSeriesOffset,
  tagSeriesShareChannelIdx, tagSeriesShareSeriesIdx,
  tagSeriesValues,
} = require('../constants/tagGuids');

// ============================================================
// Local helper functions (ported from rec_observ.cpp)
// ============================================================

/**
 * Round second values to the nearest nanosecond.
 * @param {number} dTime
 * @returns {number}
 */
function _FixSeconds(dTime) {
  return Math.floor(dTime * 1000000000.0 + 0.5) / 1000000000.0;
}

/**
 * Add seconds to a PQDIF timestamp with overflow/underflow handling.
 * @param {Object} dt - { day, sec }
 * @param {number} secondsToAdd
 */
function _AddSeconds(dt, secondsToAdd) {
  const dwDay = dt.day;

  dt.sec += secondsToAdd;

  if (dt.sec > SECONDS_PER_DAY) {
    const lDays = Math.floor(dt.sec / SECONDS_PER_DAY);
    dt.day += lDays;
    dt.sec -= lDays * SECONDS_PER_DAY;

    // Overflow check
    if (dt.day < dwDay) {
      dt.day = 0xffffffff;
      dt.sec = 86399.999999;
    }
  } else if (dt.sec < 0.0) {
    const lDays = 1 - Math.floor(dt.sec / SECONDS_PER_DAY);
    dt.day -= lDays;
    dt.sec += lDays * SECONDS_PER_DAY;

    // Underflow check
    if (dt.day > dwDay) {
      dt.day = 0;
      dt.sec = 0.0;
    }
  }

  dt.sec = _FixSeconds(dt.sec);
}

/**
 * CPQDIF_R_Observation - observation record.
 * Ported from rec_observ.h/rec_observ.cpp
 */
class ObservationRecord extends Record {
  constructor(baseRecord) {
    super();
    this._dataSource = null;   // DataSourceRecord reference
    this._settings = null;     // SettingsRecord reference (can be null)
    this._record = baseRecord || null; // Reference to underlying record

    // If baseRecord is provided, copy its state
    if (baseRecord) {
      this.posThisRecord = baseRecord.posThisRecord;
      this.headerRecord = baseRecord.headerRecord;
      this.mainCollection = baseRecord.mainCollection;
      this.changed = baseRecord.changed;
    }
  }

  // ============================================================
  // Delegation to underlying record
  // ============================================================

  readHeader(pstream) { return this._record ? this._record.readHeader(pstream) : false; }
  readBody(pstream) { return this._record ? this._record.readBody(pstream) : false; }
  writeHeader(pstream) { return this._record ? this._record.writeHeader(pstream) : false; }
  writeBody(pstream) { return this._record ? this._record.writeBody(pstream) : false; }

  // ============================================================
  // Data source / settings references
  // ============================================================

  setDataSource(pds) { this._dataSource = pds; }
  getDataSource() { return this._dataSource; }

  setMonitorSettings(psett) { this._settings = psett; }
  getMonitorSettings() { return this._settings; }

  // ============================================================
  // Read functions
  // ============================================================

  /**
   * Get observation info.
   * @param {Object} out - { timeStart, timeCreate, name }
   * @returns {boolean}
   */
  getInfo(out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    let status = true;

    // Time start
    const psc = Record.findScalarInCollection(pcollMain, tagTimeStart);
    if (psc) {
      const r = psc.getValueTimeStamp();
      if (r.status) out.timeStart = r.value;
    }

    // Time create
    const psc2 = Record.findScalarInCollection(pcollMain, tagTimeCreate);
    if (psc2) {
      const r = psc2.getValueTimeStamp();
      if (r.status) out.timeCreate = r.value;
    }

    // Name
    const pvect = Record.findVectorInCollection(pcollMain, tagObservationName);
    if (pvect) {
      out.name = pvect.getString();
    }

    return status;
  }

  /**
   * Get count of channels.
   * @returns {number}
   */
  getCountChannels() {
    const pcolCI = this.getChannelInstances();
    if (pcolCI) {
      return pcolCI.getCount();
    }
    return 0;
  }

  /**
   * Get count of series for a channel.
   * @param {number} idxChannel
   * @returns {number}
   */
  getCountSeries(idxChannel) {
    const pcolOneChannel = this.getOneChannel(idxChannel);
    if (pcolOneChannel) {
      const pcolSI = this.getSeriesInstances(pcolOneChannel);
      if (pcolSI) {
        return pcolSI.getCount();
      }
    }
    return 0;
  }

  /**
   * Get channel info by index.
   * @param {number} idxChannel
   * @param {Object} out - { name, idPhase, idQuantityType, idQuantityMeasured }
   * @returns {boolean}
   */
  getChannelInfo(idxChannel, out) {
    let idxChannelDefn = 0;
    const status = this.getChannelDefnIdx(idxChannel, { value: idxChannelDefn });
    if (!status) return false;

    idxChannelDefn = status.value !== undefined ? status.value : idxChannelDefn;

    if (this._dataSource) {
      const result = { name: '', idPhase: 0, idQuantityType: null, idQuantityMeasured: 0 };
      const dsStatus = this._dataSource.getChannelDefnInfo(idxChannelDefn, result);
      if (dsStatus) {
        out.name = result.name;
        out.idPhase = result.idPhase;
        out.idQuantityType = result.idQuantityType;
        out.idQuantityMeasured = result.idQuantityMeasured;
      }
      return dsStatus;
    }
    return false;
  }

  /**
   * Get primary series index for a channel.
   * @param {number} idxChannel
   * @param {Object} out - { idxPrimarySeries }
   * @returns {boolean}
   */
  getChannelPrimarySeries(idxChannel, out) {
    const outDefn = { value: 0 };
    const status = this.getChannelDefnIdx(idxChannel, outDefn);
    if (!status) return false;

    if (this._dataSource) {
      return this._dataSource.getChannelPrimarySeries(outDefn.value, out);
    }
    return false;
  }

  /**
   * Get series info by channel and series index.
   * @param {number} idxChannel
   * @param {number} idxSeries
   * @param {Object} out - { idQuantityUnits, idQuantityCharacteristic, idValueType }
   * @returns {boolean}
   */
  getSeriesInfo(idxChannel, idxSeries, out) {
    const outDefn = { value: 0 };
    const status = this.getChannelDefnIdx(idxChannel, outDefn);
    if (!status) return false;

    if (this._dataSource) {
      const result = { idQuantityUnits: 0, idValueType: null, idQuantityCharacteristic: null, idStorageMethod: 0 };
      const dsStatus = this._dataSource.getSeriesDefnInfo(outDefn.value, idxSeries, result);
      if (dsStatus) {
        out.idQuantityUnits = result.idQuantityUnits;
        out.idQuantityCharacteristic = result.idQuantityCharacteristic;
        out.idValueType = result.idValueType;
      }
      return dsStatus;
    }
    return false;
  }

  /**
   * Get trigger info.
   * @param {Object} out - { idTriggerMethod, pvectTriggerChanIdx, timeTriggered }
   * @returns {boolean}
   */
  getTriggerInfo(out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    let status = false;

    // Trigger method
    const psc = Record.findScalarInCollection(pcollMain, tagTriggerMethodID);
    if (psc) {
      const r = psc.getValueUINT4();
      if (r.status) {
        out.idTriggerMethod = r.value;
        status = true;
      }
    }

    // Triggered channels (optional)
    if (status && out.idTriggerMethod === ID_TRIGGER_METH_CHANNEL) {
      const pvect = Record.findVectorInCollection(pcollMain, tagChannelTriggerIdx);
      if (pvect && pvect.getPhysicalType() === ID_PHYS_TYPE_UNS_INTEGER4) {
        out.pvectTriggerChanIdx = pvect;
      }
    }

    // Time triggered
    status = false;
    const psc2 = Record.findScalarInCollection(pcollMain, tagTimeTriggered);
    if (psc2) {
      const r = psc2.getValueTimeStamp();
      if (r.status) {
        out.timeTriggered = r.value;
        status = true;
      }
    }

    return status;
  }

  /**
   * Get series base quantity.
   * @param {number} idxChannel
   * @param {number} idxSeries
   * @param {Object} out - { value }
   * @returns {boolean}
   */
  getSeriesBaseQuantity(idxChannel, idxSeries, out) {
    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (!pcol) return false;

    const psc = Record.findScalarInCollection(pcol, tagSeriesBaseQuantity);
    if (psc) {
      const r = psc.getValueREAL8();
      if (r.status) {
        out.value = r.value;
        return true;
      }
    }
    return false;
  }

  /**
   * Get series scale and offset.
   * @param {number} idxChannel
   * @param {number} idxSeries
   * @param {Object} out - { scale, offset }
   * @returns {boolean}
   */
  getSeriesScale(idxChannel, idxSeries, out) {
    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (!pcol) return false;

    let status = false;

    const pscScale = Record.findScalarInCollection(pcol, tagSeriesScale);
    if (pscScale) {
      const r = pscScale.getValueREAL8();
      if (r.status) {
        out.scale = r.value;
        status = true;
      }
    }

    const pscOffset = Record.findScalarInCollection(pcol, tagSeriesOffset);
    if (pscOffset) {
      const r = pscOffset.getValueREAL8();
      if (r.status) {
        out.offset = r.value;
        status = true;
      }
    }

    return status;
  }

  /**
   * Get nominal value from data source.
   */
  getSeriesDefnNominal(idxChannel, idxSeries, out) {
    let idxChannelDefn = 0;
    const status = this.getChannelDefnIdx(idxChannel, { value: idxChannelDefn });
    if (!status) return false;

    if (this._dataSource) {
      return this._dataSource.getSeriesDefnNominal(status.value !== undefined ? status.value : idxChannelDefn, idxSeries, out);
    }
    return false;
  }

  /**
   * Get precision and resolution from data source.
   */
  getSeriesDefnPrecisionAndResolution(idxChannel, idxSeries, out) {
    let idxChannelDefn = 0;
    const status = this.getChannelDefnIdx(idxChannel, { value: idxChannelDefn });
    if (!status) return false;

    if (this._dataSource) {
      return this._dataSource.getSeriesDefnPrecisionAndResolution(
        status.value !== undefined ? status.value : idxChannelDefn, idxSeries, out);
    }
    return false;
  }

  /**
   * Get the raw series value vector.
   * @param {number} idxChannel
   * @param {number} idxSeries
   * @returns {Vector|null}
   */
  getSeriesValueVector(idxChannel, idxSeries) {
    const pcolOneSeries = this.getOneSeries(idxChannel, idxSeries);
    if (pcolOneSeries) {
      return Record.findVectorInCollection(pcolOneSeries, tagSeriesValues);
    }
    return null;
  }

  // ============================================================
  // Write functions
  // ============================================================

  /**
   * Add a channel instance.
   * @param {number} idxChannelDefn
   * @returns {number} index of new channel
   */
  addChannel(idxChannelDefn) {
    const pcolInstances = this.getChannelInstances();
    if (!pcolInstances) return -1;

    const idxNew = this.getCountChannels();

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneChannelInst);
      pcollOne.setScalarUINT4(tagChannelDefnIdx, idxChannelDefn);

      const pcollSeriesInstances = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcollSeriesInstances) {
        pcollSeriesInstances.setTag(tagSeriesInstances);
        pcollOne.add(pcollSeriesInstances);
      }

      pcolInstances.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Add a series with double values.
   */
  addSeriesDouble(idxChannel, countValues, arValues) {
    const factory = require('./factory');
    const pvectData = factory.newElement(ID_ELEMENT_TYPE_VECTOR);
    if (!pvectData) return -1;

    pvectData.setTag(tagSeriesValues);
    pvectData.setPhysicalType(ID_PHYS_TYPE_REAL8);
    pvectData.setCount(countValues);
    for (let idx = 0; idx < countValues; idx++) {
      pvectData.setValue(idx, arValues[idx]);
    }

    const idxNew = this.addSeriesVector(idxChannel, pvectData);
    return idxNew;
  }

  /**
   * Add a series with a vector.
   */
  addSeriesVector(idxChannel, pvectData) {
    const pcolOneChannel = this.getOneChannel(idxChannel);
    if (!pcolOneChannel || !pvectData) return -1;

    const pcolSI = this.getSeriesInstances(pcolOneChannel);
    if (!pcolSI) return -1;

    const idxNew = pcolSI.getCount();

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneSeriesInstance);
      pvectData.setTag(tagSeriesValues);
      pcollOne.add(pvectData);
      pcolSI.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Add a shared series.
   */
  addSeriesShared(idxChannel, idxChannelShared, idxSeriesShared) {
    const pcolOneChannel = this.getOneChannel(idxChannel);
    if (!pcolOneChannel) return -1;

    const pcolSI = this.getSeriesInstances(pcolOneChannel);
    if (!pcolSI) return -1;

    const idxNew = pcolSI.getCount();

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneSeriesInstance);

      pcollOne.setScalarUINT4(tagSeriesShareChannelIdx, idxChannelShared);
      pcollOne.setScalarUINT4(tagSeriesShareSeriesIdx, idxSeriesShared);

      pcolSI.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Set series base quantity.
   */
  setSeriesBaseQuantity(idxChannel, idxSeries, value) {
    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (!pcol) return false;

    const factory = require('./factory');
    let psc = Record.findScalarInCollection(pcol, tagSeriesBaseQuantity);
    if (!psc) {
      psc = factory.newElement(ID_ELEMENT_TYPE_SCALAR);
      if (psc) pcol.add(psc);
    }

    if (psc) {
      psc.setTag(tagSeriesBaseQuantity);
      psc.setValueREAL8(value);
      return true;
    }
    return false;
  }

  /**
   * Set series scale and offset.
   */
  setSeriesScale(idxChannel, idxSeries, scale, offset) {
    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (!pcol) return false;

    const factory = require('./factory');

    // Scale
    let psc = Record.findScalarInCollection(pcol, tagSeriesScale);
    if (!psc) {
      psc = factory.newElement(ID_ELEMENT_TYPE_SCALAR);
      if (psc) {
        psc.setTag(tagSeriesScale);
        pcol.add(psc);
      }
    }
    if (psc) psc.setValueREAL8(scale);

    // Offset
    psc = Record.findScalarInCollection(pcol, tagSeriesOffset);
    if (!psc) {
      psc = factory.newElement(ID_ELEMENT_TYPE_SCALAR);
      if (psc) {
        psc.setTag(tagSeriesOffset);
        pcol.add(psc);
      }
    }
    if (psc) psc.setValueREAL8(offset);

    return true;
  }

  /**
   * Set trigger info.
   */
  setTriggerInfo(idTriggerMethod, countTriggers, aidxTriggerChan, timeTriggered) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    let status = false;
    const factory = require('./factory');

    // Set trigger method
    const psc = Record.findOrCreateScalarInCollection(pcollMain, tagTriggerMethodID, ID_PHYS_TYPE_UNS_INTEGER4);
    if (psc) {
      status = psc.setValueUINT4(idTriggerMethod);
    }

    // Set triggered channels
    if (status && idTriggerMethod === ID_TRIGGER_METH_CHANNEL) {
      const pvect = Record.findOrCreateVectorInCollection(pcollMain, tagChannelTriggerIdx, ID_PHYS_TYPE_UNS_INTEGER4);
      if (pvect && pvect.getPhysicalType() === ID_PHYS_TYPE_UNS_INTEGER4) {
        pvect.setCount(countTriggers);
        for (let idx = 0; idx < countTriggers; idx++) {
          pvect.setValueUINT4(idx, aidxTriggerChan[idx]);
        }
      }
    }

    // Set time triggered
    if (status) {
      const psc2 = Record.findOrCreateScalarInCollection(pcollMain, tagTimeTriggered, ID_PHYS_TYPE_TIMESTAMPPQDIF);
      if (psc2) {
        status = psc2.setValueTimeStamp(timeTriggered);
      }
    }

    return status;
  }

  // ============================================================
  // Extended data access
  // ============================================================

  getObservationExtendedData(gidTag, out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    const psc = Record.findScalarInCollection(pcollMain, gidTag);
    if (psc) {
      return psc.getValue(out);
    }
    return false;
  }

  getChannelExtendedData(idxChannel, gidTag, out) {
    const pcollOneChannel = this.getOneChannel(idxChannel);
    if (!pcollOneChannel) return false;

    const psc = Record.findScalarInCollection(pcollOneChannel, gidTag);
    if (psc) {
      return psc.getValue(out);
    }
    return false;
  }

  getSeriesExtendedData(idxChannel, idxSeries, gidTag, out) {
    const pcolOneSeries = this.getOneSeries(idxChannel, idxSeries);
    if (!pcolOneSeries) return false;

    const psc = Record.findScalarInCollection(pcolOneSeries, gidTag);
    if (psc) {
      return psc.getValue(out);
    }
    return false;
  }

  // ============================================================
  // Series resolution (critical path - ported carefully)
  // ============================================================

  /**
   * Get count of resolved series points.
   */
  getCountResolvedSeries(idxChannel, idxSeries, noShare) {
    if (!this._record || !this._dataSource || idxChannel < 0 || idxSeries < 0) {
      return 0;
    }

    const outDefn = { value: 0 };
    const status = this.getChannelDefnIdx(idxChannel, outDefn);
    if (!status || outDefn.value < 0) return 0;

    // Get storage method
    const seriesDefnInfo = { idQuantityUnits: 0, idValueType: null, idQuantityCharacteristic: null, idStorageMethod: 0 };
    const dsStatus = this._dataSource.getSeriesDefnInfo(outDefn.value, idxSeries, seriesDefnInfo);
    if (!dsStatus) return 0;

    const idStorageMethod = seriesDefnInfo.idStorageMethod;

    // Look for shared channel/series or vector of values
    let gotShareChannel = false;
    let gotShareSeries = false;
    let idxShareChannelIdx = 0;
    let idxShareSeriesIdx = 0;
    let pvectSeriesArray = null;

    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (pcol) {
      for (let idx = 0; idx < pcol.getCount(); idx++) {
        const pel = pcol.getElement(idx);
        if (!pel) continue;

        if (pel.getElementType() === ID_ELEMENT_TYPE_SCALAR) {
          if (guidEquals(pel.getTag(), tagSeriesShareChannelIdx)) {
            gotShareChannel = true;
            const r = pel.getValueUINT4();
            if (r.status) idxShareChannelIdx = r.value;
            if (gotShareSeries) break;
          }
          if (guidEquals(pel.getTag(), tagSeriesShareSeriesIdx)) {
            gotShareSeries = true;
            const r = pel.getValueUINT4();
            if (r.status) idxShareSeriesIdx = r.value;
            if (gotShareChannel) break;
          }
        } else if (pel.getElementType() === ID_ELEMENT_TYPE_VECTOR &&
                   guidEquals(pel.getTag(), tagSeriesValues)) {
          pvectSeriesArray = pel;
          break;
        }
      }
    }

    // Count the values
    if (gotShareChannel && gotShareSeries) {
      if (!noShare) {
        return this.getCountResolvedSeries(idxShareChannelIdx, idxShareSeriesIdx, true);
      }
      return 0;
    } else if (pvectSeriesArray) {
      return this._generateSeriesCount(idStorageMethod, pvectSeriesArray);
    }

    return 0;
  }

  /**
   * Get resolved series data as a double array.
   * Caller must delete the returned array.
   * @returns {Float64Array|null}
   */
  newResolvedSeries(idxChannel, idxSeries, noShare) {
    if (!this._record || !this._dataSource || idxChannel < 0 || idxSeries < 0) {
      return null;
    }

    const outDefn = { value: 0 };
    const status = this.getChannelDefnIdx(idxChannel, outDefn);
    if (!status || outDefn.value < 0) return null;

    // Gather channel and series definition info
    const chanDefnInfo = { name: '', idPhase: 0, idQuantityType: null, idQuantityMeasured: 0 };
    if (!this._dataSource.getChannelDefnInfo(outDefn.value, chanDefnInfo)) return null;

    const seriesDefnInfo = { idQuantityUnits: 0, idValueType: null, idQuantityCharacteristic: null, idStorageMethod: 0 };
    if (!this._dataSource.getSeriesDefnInfo(outDefn.value, idxSeries, seriesDefnInfo)) return null;

    const idStorageMethod = seriesDefnInfo.idStorageMethod;

    let rBaseValue = 1.0;
    let rScale = 1.0;
    let rOffset = 0.0;
    let gotShareChannel = false;
    let gotShareSeries = false;
    let idxShareChannelIdx = 0;
    let idxShareSeriesIdx = 0;
    let pvectSeriesArray = null;

    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (pcol) {
      for (let idx = 0; idx < pcol.getCount(); idx++) {
        const pel = pcol.getElement(idx);
        if (!pel) continue;

        if (pel.getElementType() === ID_ELEMENT_TYPE_SCALAR) {
          const psc = pel;

          // Extract REAL4 or REAL8 values
          let valueReal = 0.0;
          const outVal = { typePhysical: 0, value: null };
          psc.getValue(outVal);
          if (outVal.typePhysical === ID_PHYS_TYPE_REAL4) valueReal = outVal.value;
          if (outVal.typePhysical === ID_PHYS_TYPE_REAL8) valueReal = outVal.value;

          if (guidEquals(pel.getTag(), tagSeriesBaseQuantity)) rBaseValue = valueReal;
          if (guidEquals(pel.getTag(), tagSeriesScale)) rScale = valueReal;
          if (guidEquals(pel.getTag(), tagSeriesOffset)) rOffset = valueReal;

          if (guidEquals(pel.getTag(), tagSeriesShareChannelIdx)) {
            gotShareChannel = true;
            const r = psc.getValueUINT4();
            if (r.status) idxShareChannelIdx = r.value;
          }
          if (guidEquals(pel.getTag(), tagSeriesShareSeriesIdx)) {
            gotShareSeries = true;
            const r = psc.getValueUINT4();
            if (r.status) idxShareSeriesIdx = r.value;
          }
        }

        if (pel.getElementType() === ID_ELEMENT_TYPE_VECTOR &&
            guidEquals(pel.getTag(), tagSeriesValues)) {
          pvectSeriesArray = pel;
        }
      }
    }

    // Generate the series data
    if (gotShareChannel && gotShareSeries) {
      if (!noShare) {
        return this.newResolvedSeries(idxShareChannelIdx, idxShareSeriesIdx, true);
      }
      return null;
    } else if (pvectSeriesArray) {
      const outCount = { value: 0 };
      return this._generateSeriesData(outDefn.value, idStorageMethod,
        rBaseValue, rScale, rOffset, pvectSeriesArray, outCount);
    }

    return null;
  }

  /**
   * Get resolved series count (non-C++ style returning count via out param).
   */
  newResolvedSeriesCount(idxChannel, idxSeries, outCount, noShare) {
    const result = this.newResolvedSeries(idxChannel, idxSeries, noShare);
    if (result) {
      outCount.value = result.length;
    }
    return result;
  }

  /**
   * Get resolved series timestamp array.
   * @returns {Array|null}
   */
  newResolvedSeriesTimeStamp(idxChannel, idxSeries, noShare) {
    const pcol = this.getOneSeries(idxChannel, idxSeries);
    if (!pcol) return null;

    // Check for shared series
    let gotShareChannel = false;
    let gotShareSeries = false;
    let idxShareChannelIdx = 0;
    let idxShareSeriesIdx = 0;

    const pelChannel = pcol.getElementByTag(tagSeriesShareChannelIdx, ID_ELEMENT_TYPE_SCALAR);
    if (pelChannel) {
      gotShareChannel = true;
      const r = pelChannel.getValueUINT4();
      if (r.status) idxShareChannelIdx = r.value;
    }

    const pelSeries = pcol.getElementByTag(tagSeriesShareSeriesIdx, ID_ELEMENT_TYPE_SCALAR);
    if (pelSeries) {
      gotShareSeries = true;
      const r = pelSeries.getValueUINT4();
      if (r.status) idxShareSeriesIdx = r.value;
    }

    if (gotShareChannel && gotShareSeries && !noShare) {
      // Shared - recurse once
      return this.newResolvedSeriesTimeStamp(idxShareChannelIdx, idxShareSeriesIdx, true);
    }

    // Not shared - try to decode normally
    const pelValues = pcol.getElementByTag(tagSeriesValues, ID_ELEMENT_TYPE_VECTOR);
    if (!pelValues || pelValues.getElementType() !== ID_ELEMENT_TYPE_VECTOR) {
      return null;
    }

    const pvectSeriesArray = pelValues;

    // Already in timestamp form?
    if (pvectSeriesArray.getPhysicalType() === ID_PHYS_TYPE_TIMESTAMPPQDIF) {
      const countPoints = pvectSeriesArray.getCount();
      const result = [];
      for (let i = 0; i < countPoints; i++) {
        const pv = pvectSeriesArray.getValue(i);
        if (pv) result.push(pv.value);
      }
      return result;
    }

    // Fabricate from starting timestamp and second offsets
    const seconds = this.newResolvedSeries(idxChannel, idxSeries, true);
    if (!seconds) return null;

    const result = this._generateTimeStampArray(seconds);
    return result;
  }

  // ============================================================
  // Internal functions
  // ============================================================

  /**
   * Get channel definition index from a channel instance.
   */
  getChannelDefnIdx(idxChannel, out) {
    const pcollOneChannel = this.getOneChannel(idxChannel);
    if (!pcollOneChannel) return false;

    const psc = Record.findScalarInCollection(pcollOneChannel, tagChannelDefnIdx);
    if (psc) {
      const r = psc.getValueUINT4();
      if (r.status) {
        out.value = r.value;
        return true;
      }
    }
    return false;
  }

  /**
   * Get channel instances collection (create if not exists).
   */
  getChannelInstances() {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return null;

    let pcollChannels = Record.findCollectionInCollection(pcollMain, tagChannelInstances);
    if (!pcollChannels) {
      const factory = require('./factory');
      pcollChannels = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcollChannels) {
        pcollChannels.setTag(tagChannelInstances);
        pcollMain.add(pcollChannels);
      }
    }

    return pcollChannels;
  }

  /**
   * Get one channel instance by index.
   */
  getOneChannel(idxChannel) {
    const pcolInstances = this.getChannelInstances();
    if (!pcolInstances) return null;

    const pel = pcolInstances.getElement(idxChannel);
    if (pel &&
        pel.getElementType() === ID_ELEMENT_TYPE_COLLECTION &&
        pel.getTag() && guidEquals(pel.getTag(), tagOneChannelInst)) {
      return pel;
    }

    return null;
  }

  /**
   * Get series instances collection for a channel.
   */
  getSeriesInstances(pcolChannel) {
    return Record.findCollectionInCollection(pcolChannel, tagSeriesInstances);
  }

  /**
   * Get one series instance by channel and series index.
   */
  getOneSeriesByChannel(pcolChannel, idxSeries) {
    const pcolSeriesInstances = this.getSeriesInstances(pcolChannel);
    if (!pcolSeriesInstances) return null;

    const pel = pcolSeriesInstances.getElement(idxSeries);
    if (pel &&
        pel.getElementType() === ID_ELEMENT_TYPE_COLLECTION &&
        pel.getTag() && guidEquals(pel.getTag(), tagOneSeriesInstance)) {
      return pel;
    }

    return null;
  }

  /**
   * Get one series instance by channel and series indices.
   */
  getOneSeries(idxChannel, idxSeries) {
    const pcolOneChannel = this.getOneChannel(idxChannel);
    if (!pcolOneChannel) return null;
    return this.getOneSeriesByChannel(pcolOneChannel, idxSeries);
  }

  /**
   * Generate series count for a storage method.
   */
  _generateSeriesCount(idStorageMethod, pvectSeriesArray) {
    let countPoints = 0;

    if (idStorageMethod & ID_SERIES_METHOD_VALUES) {
      countPoints = pvectSeriesArray.getCount();
    } else if (idStorageMethod & ID_SERIES_METHOD_INCREMENT) {
      const countValues = pvectSeriesArray.getCount();
      if (countValues > 0) {
        for (let idxValue = 1; idxValue < countValues; idxValue += 2) {
          const r = pvectSeriesArray.getValueAsDouble(idxValue);
          if (r.status) countPoints += Math.floor(r.value);
        }
      }
    }

    return countPoints;
  }

  /**
   * Generate series data array.
   */
  _generateSeriesData(idxChannelDefn, idStorageMethod, rBaseValue, rScale, rOffset, pvectSeriesArray, outCount) {
    const countValues = pvectSeriesArray.getCount();
    const countPoints = this._generateSeriesCount(idStorageMethod, pvectSeriesArray);
    outCount.value = countPoints;

    let arValues = null;

    if (idStorageMethod & ID_SERIES_METHOD_VALUES) {
      // Straight vector
      if (countPoints > 0 && countPoints === countValues) {
        arValues = new Float64Array(countPoints);
        for (let idxPoint = 0; idxPoint < countPoints; idxPoint++) {
          const r = pvectSeriesArray.getValueAsDouble(idxPoint);
          arValues[idxPoint] = r.status ? r.value : 0.0;
        }
      }
    } else if (idStorageMethod & ID_SERIES_METHOD_INCREMENT) {
      // Incremental storage method
      if (countPoints > 0) {
        arValues = new Float64Array(countPoints);
        let valueNext = 0.0;
        let idxPoint = 0;

        // Get number of rate changes
        const rCount0 = pvectSeriesArray.getValueAsDouble(0);
        const countChanges = rCount0.status ? Math.floor(rCount0.value) : 0;

        for (let idxChange = 0; idxChange < countChanges; idxChange++) {
          const idxValue = (idxChange * 2) + 1;
          const rCount = pvectSeriesArray.getValueAsDouble(idxValue);
          const rRate = pvectSeriesArray.getValueAsDouble(idxValue + 1);
          const countPointsThisChange = rCount.status ? Math.floor(rCount.value) : 0;
          const rate = rRate.status ? rRate.value : 0;

          for (let i = 0; i < countPointsThisChange && idxPoint < countPoints; i++, idxPoint++) {
            arValues[idxPoint] = valueNext;
            valueNext += rate;
          }
        }
      }
    }

    // Apply scale/offset?
    let useScale = false;
    let useCal = false;

    if (idStorageMethod & ID_SERIES_METHOD_SCALED) {
      useScale = true;
    } else {
      rScale = 1.0;
      rOffset = 0.0;
    }

    // CT/PT ratios
    const calResult = { value: rScale };
    useCal = this._getCalibrationRatio(idxChannelDefn, calResult);
    if (useCal) {
      rScale = calResult.value;
      useScale = true;
    }

    // Apply scale and offset
    if (useScale && arValues && countPoints > 0) {
      for (let idxPoint = 0; idxPoint < countPoints; idxPoint++) {
        arValues[idxPoint] = arValues[idxPoint] * rScale + rOffset;
      }
    }

    return arValues;
  }

  /**
   * Generate timestamp array from seconds offsets.
   */
  _generateTimeStampArray(seconds) {
    const outInfo = { timeStart: { day: 0, sec: 0.0 }, timeCreate: { day: 0, sec: 0.0 }, name: '' };
    const status = this.getInfo(outInfo);
    if (!status) return null;

    const tsStart = outInfo.timeStart;
    if (!tsStart || tsStart.day === undefined) return null;

    const countPoints = seconds.length;
    const result = [];

    for (let idx = 0; idx < countPoints; idx++) {
      const tsTemp = { day: tsStart.day, sec: tsStart.sec };
      _AddSeconds(tsTemp, seconds[idx]);
      result.push(tsTemp);
    }

    return result;
  }

  /**
   * Get calibration ratio from settings.
   */
  _GetCalibrationRatio(idxChannelDefn, rScale) {
    if (!this._settings) return false;

    const outInfo = { timeEffective: { day: 0, sec: 0.0 }, timeInstalled: { day: 0, sec: 0.0 }, timeRemoved: { day: 0, sec: 0.0 }, useCal: false, useTrans: false };
    const gotInfo = this._settings.getInfo(outInfo);
    if (!gotInfo || !outInfo.useCal) return false;

    const countChannels = this._settings.getCountChannels();
    for (let idxChannel = 0; idxChannel < countChannels; idxChannel++) {
      const outDefn = { idxChannelDefn: 0 };
      const gotDefn = this._settings.getChannelInfo(idxChannel, outDefn);
      if (gotDefn && idxChannelDefn === outDefn.idxChannelDefn) {
        const outTrans = { xdTransformerTypeID: 0, xdSystemSideRatio: 0, xdMonitorSideRatio: 0, xdFrequencyResponse: [] };
        const gotTrans = this._settings.getChanTrans(idxChannel, outTrans);
        if (gotTrans) {
          rScale.value *= outTrans.xdMonitorSideRatio;
          rScale.value /= outTrans.xdSystemSideRatio;
          return true;
        }
      }
    }

    return false;
  }

  _getCalibrationRatio(idxChannelDefn, rScale) {
    return this._GetCalibrationRatio(idxChannelDefn, rScale);
  }
}

module.exports = { ObservationRecord };
