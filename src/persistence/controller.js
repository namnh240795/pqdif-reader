'use strict';

/**
 * Base persistence controller.
 * Ported from pcn_base.h / pcn_base.cpp
 *
 * Manages an array of PQDIF records and provides methods for creating
 * standard record types (Container, DataSource, MonitorSettings, Observation).
 *
 * Subclasses (FlatFileController) implement the actual I/O for reading and
 * writing records to a specific storage medium.
 */
class PersistenceController {
  constructor() {
    /** @type {Array} Array of CPQDIFRecord objects */
    this.records = [];

    /**
     * State of the controller:
     * - 'empty': newly created
     * - 'matchesFile': contents match what was read from disk
     * - 'modified': contents have been modified in memory
     * @type {string}
     */
    this.state = 'empty';
  }

  // -----------------------------------------------------------------------
  //  Record management
  // -----------------------------------------------------------------------

  /**
   * Get the number of records.
   * @returns {number}
   */
  getRecordCount() {
    return this.records.length;
  }

  /**
   * Get a record by index.
   * @param {number} index
   * @returns {Object|null} The record, or null if out of range
   */
  getRecord(index) {
    if (index >= 0 && index < this.records.length) {
      return this.records[index];
    }
    return null;
  }

  /**
   * Insert a record at the specified index.
   * @param {Object} record - The record to insert
   * @param {number} index - Index at which to insert
   * @returns {boolean} True on success
   */
  insertRecord(record, index) {
    if (index >= 0 && index <= this.records.length) {
      this.records.splice(index, 0, record);
      this.state = 'modified';
      return true;
    }
    return false;
  }

  /**
   * Remove a record at the specified index.
   * @param {number} index
   * @returns {boolean} True on success
   */
  removeRecord(index) {
    if (index >= 0 && index < this.records.length) {
      this.records.splice(index, 1);
      this.state = 'modified';
      return true;
    }
    return false;
  }

  // -----------------------------------------------------------------------
  //  Record creation methods
  // -----------------------------------------------------------------------

  /**
   * Create a Container record with version info and filename.
   * Matches C++ CreateContainerRecord(szFileName, timeCreate, ...).
   *
   * @param {string} szFileName - Filename
   * @param {Object} timeCreate - { day, sec } timestamp
   * @param {number} lMajor - Major version
   * @param {number} lMinor - Minor version
   * @param {number} lCompatMajor - Compatibility major version
   * @param {number} lCompatMinor - Compatibility minor version
   * @returns {number} Index of the new record (always 0)
   */
  createContainerRecord(filename, versionInfo, timeCreate, lMajor, lMinor, lCompatMajor, lCompatMinor) {
    const factory = require('../core/factory');
    const tagGuids = require('../constants/tagGuids');
    const { PqdifValue } = require('../core/pqdifValue');

    const prec = factory.newRecord('Container');
    if (!prec) return -1;

    const pcollMain = factory.newElement(1); // ID_ELEMENT_TYPE_COLLECTION
    if (!pcollMain) return -1;

    // Add filename as a vector of CHAR1
    if (filename) {
      pcollMain.setVectorString(tagGuids.tagFileName, filename);
    }

    // Add creation timestamp
    if (timeCreate) {
      pcollMain.setScalarTimeStamp(tagGuids.tagCreation, timeCreate);
    }

    // Add version info as a vector of UINT4
    if (versionInfo) {
      pcollMain.setVectorUINT4(tagGuids.tagVersionInfo, versionInfo, 4);
    }

    prec.setMainCollection(pcollMain);
    this.records[0] = prec;
    return 0;
  }

  /**
   * Create a Container record with metadata strings.
   * Matches C++ CreateContainerRecord(language, title, ...).
   *
   * @param {string} language
   * @param {string} title
   * @param {string} subject
   * @param {string} author
   * @param {string} keywords
   * @param {string} comments
   * @param {string} lastSavedBy
   * @param {string} application
   * @param {string} security
   * @param {string} owner
   * @param {string} copyright
   * @param {string} trademarks
   * @param {string} notes
   * @returns {number} Index of the new record (always 0)
   */
  createContainerRecordWithMeta(language, title, subject, author, keywords, comments,
    lastSavedBy, application, security, owner, copyright, trademarks, notes) {
    const factory = require('../core/factory');
    const tagGuids = require('../constants/tagGuids');

    const prec = factory.newRecord('Container');
    if (!prec) return -1;

    const pcollMain = factory.newElement(1); // ID_ELEMENT_TYPE_COLLECTION
    if (!pcollMain) return -1;

    // Set all string entries
    if (language) pcollMain.setVectorString(tagGuids.tagLanguage, language);
    if (title) pcollMain.setVectorString(tagGuids.tagTitle, title);
    if (subject) pcollMain.setVectorString(tagGuids.tagSubject, subject);
    if (author) pcollMain.setVectorString(tagGuids.tagAuthor, author);
    if (keywords) pcollMain.setVectorString(tagGuids.tagKeywords, keywords);
    if (comments) pcollMain.setVectorString(tagGuids.tagComments, comments);
    if (lastSavedBy) pcollMain.setVectorString(tagGuids.tagLastSavedBy, lastSavedBy);
    if (application) pcollMain.setVectorString(tagGuids.tagApplication, application);
    if (security) pcollMain.setVectorString(tagGuids.tagSecurity, security);
    if (owner) pcollMain.setVectorString(tagGuids.tagOwner, owner);
    if (copyright) pcollMain.setVectorString(tagGuids.tagCopyright, copyright);
    if (trademarks) pcollMain.setVectorString(tagGuids.tagTrademarks, trademarks);
    if (notes) pcollMain.setVectorString(tagGuids.tagNotes, notes);

    prec.setMainCollection(pcollMain);
    this.records[0] = prec;
    return 0;
  }

  /**
   * Create a DataSource record.
   * Matches C++ CreateDataSourceRecord(indexInsert, ...).
   *
   * @param {number} indexInsert - Index at which to insert
   * @param {Buffer} typeGuid - DataSource type GUID (16 bytes)
   * @param {Buffer} vendorGuid - Vendor GUID (16 bytes)
   * @param {Buffer} equipmentGuid - Equipment GUID (16 bytes)
   * @param {string} serialNumber - Serial number string
   * @param {string} version - Version string
   * @param {string} name - Data source name
   * @param {string} owner - Owner string
   * @param {string} location - Location string
   * @param {string} timezone - Timezone string
   * @returns {number} Index of the new record
   */
  createDataSourceRecord(indexInsert, typeGuid, vendorGuid, equipmentGuid,
    serialNumber, version, name, owner, location, timezone) {
    const factory = require('../core/factory');
    const tagGuids = require('../constants/tagGuids');
    const { PqdifValue } = require('../core/pqdifValue');

    const prec = factory.newRecord('DataSource');
    if (!prec) return -1;

    const pcollMain = factory.newElement(1); // ID_ELEMENT_TYPE_COLLECTION
    if (!pcollMain) return -1;

    // Add GUIDs as scalars
    if (typeGuid) {
      pcollMain.setScalarGUID(tagGuids.tagDataSourceTypeID, typeGuid);
    }
    if (vendorGuid) {
      pcollMain.setScalarGUID(tagGuids.tagVendorID, vendorGuid);
    }
    if (equipmentGuid) {
      pcollMain.setScalarGUID(tagGuids.tagEquipmentID, equipmentGuid);
    }

    // Add string entries
    if (serialNumber) pcollMain.setVectorString(tagGuids.tagSerialNumberDS, serialNumber);
    if (version) pcollMain.setVectorString(tagGuids.tagVersionDS, version);
    if (name) pcollMain.setVectorString(tagGuids.tagNameDS, name);
    if (owner) pcollMain.setVectorString(tagGuids.tagOwnerDS, owner);
    if (location) pcollMain.setVectorString(tagGuids.tagLocationDS, location);
    if (timezone) pcollMain.setVectorString(tagGuids.tagTimeZoneDS, timezone);

    // Add empty channel definitions collection
    const pcollDefinitions = factory.newElement(1);
    if (pcollDefinitions) {
      pcollDefinitions.setTag(tagGuids.tagChannelDefns);
      pcollMain.add(pcollDefinitions);
    }

    prec.setMainCollection(pcollMain);
    this.insertRecord(prec, indexInsert);
    return indexInsert;
  }

  /**
   * Create a MonitorSettings record.
   * Matches C++ CreateMonitorSettingsRecord(indexInsert, ...).
   *
   * @param {number} indexInsert - Index at which to insert
   * @param {Object} [timeEffective] - { day, sec } effective timestamp
   * @param {Object} [timeInstalled] - { day, sec } installed timestamp
   * @param {Object} [timeRemoved] - { day, sec } removed timestamp
   * @param {boolean} [useCalibration] - Use calibration flag
   * @param {boolean} [useTransducer] - Use transducer flag
   * @returns {number} Index of the new record
   */
  createMonitorSettingsRecord(indexInsert, timeEffective, timeInstalled, timeRemoved,
    useCalibration, useTransducer) {
    const factory = require('../core/factory');

    const prec = factory.newRecord('MonitorSettings');
    if (!prec) return -1;

    const pcollMain = factory.newElement(1);
    if (!pcollMain) return -1;

    prec.setMainCollection(pcollMain);

    // Set monitor settings info if provided
    if (prec.setInfo) {
      prec.setInfo(
        timeEffective || null,
        timeInstalled || null,
        timeRemoved || null,
        useCalibration || false,
        useTransducer || false
      );
    }

    this.insertRecord(prec, indexInsert);
    return indexInsert;
  }

  /**
   * Create an Observation record.
   * Matches C++ CreateObservationRecord(indexInsert, ...).
   *
   * @param {number} indexInsert - Index at which to insert
   * @param {string} name - Observation name
   * @param {Object} timeCreate - { day, sec } creation timestamp
   * @param {Object} timeStart - { day, sec } start timestamp
   * @param {number} triggerMethod - Trigger method ID
   * @param {Object} [timeTriggered] - { day, sec } triggered timestamp (optional)
   * @param {number} [countTriggers] - Number of channel triggers (optional)
   * @param {Array<number>} [channelTriggerIndices] - Array of channel trigger indices (optional)
   * @returns {number} Index of the new record
   */
  createObservationRecord(indexInsert, name, timeCreate, timeStart, triggerMethod,
    timeTriggered, countTriggers, channelTriggerIndices) {
    const factory = require('../core/factory');
    const tagGuids = require('../constants/tagGuids');
    const { PqdifValue } = require('../core/pqdifValue');
    const { ID_PHYS_TYPE_UNS_INTEGER4 } = require('../constants/physicalTypes');

    const prec = factory.newRecord('Observation');
    if (!prec) return -1;

    const pcollMain = factory.newElement(1);
    if (!pcollMain) return -1;

    // Add observation entries
    if (name) pcollMain.setVectorString(tagGuids.tagObservationName, name);
    if (timeCreate) pcollMain.setScalarTimeStamp(tagGuids.tagTimeCreate, timeCreate);
    if (timeStart) pcollMain.setScalarTimeStamp(tagGuids.tagTimeStart, timeStart);
    pcollMain.setScalarUINT4(tagGuids.tagTriggerMethodID, triggerMethod);

    // Optional time triggered
    if (timeTriggered) {
      pcollMain.setScalarTimeStamp(tagGuids.tagTimeTriggered, timeTriggered);
    }

    // Optional channel trigger indices vector
    if (countTriggers > 0 && channelTriggerIndices) {
      const pvectChannelIdx = factory.newElement(3); // ID_ELEMENT_TYPE_VECTOR
      if (pvectChannelIdx) {
        pvectChannelIdx.setTag(tagGuids.tagChannelTriggerIdx);
        pvectChannelIdx.setPhysicalType(ID_PHYS_TYPE_UNS_INTEGER4);
        pvectChannelIdx.setCount(countTriggers);

        for (let i = 0; i < countTriggers; i++) {
          pvectChannelIdx.setValue(i, PqdifValue.fromUint(channelTriggerIndices[i]));
        }

        pcollMain.add(pvectChannelIdx);
      }
    }

    // Add empty channel instances collection
    const pcollInstances = factory.newElement(1);
    if (pcollInstances) {
      pcollInstances.setTag(tagGuids.tagChannelInstances);
      pcollMain.add(pcollInstances);
    }

    prec.setMainCollection(pcollMain);
    this.insertRecord(prec, indexInsert);
    return indexInsert;
  }
}

module.exports = PersistenceController;
