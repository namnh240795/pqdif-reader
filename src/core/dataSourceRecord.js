'use strict';

const { Record } = require('./record');
const { Collection } = require('./collection');
const { PqdifInfo, guidEquals } = require('../info/info');
const {
  ID_PHYS_TYPE_UNS_INTEGER4, ID_PHYS_TYPE_REAL8,
} = require('../constants/physicalTypes');
const { ID_ELEMENT_TYPE_COLLECTION } = require('../constants/elementTypes');
const {
  tagDataSourceTypeID, tagVendorID, tagEquipmentID,
  tagSerialNumberDS, tagVersionDS, tagNameDS, tagOwnerDS, tagLocationDS, tagTimeZoneDS,
  tagChannelDefns, tagOneChannelDefn, tagChannelName, tagPhaseID,
  tagQuantityTypeID, tagQuantityMeasuredID, tagPrimarySeriesIdx,
  tagSeriesDefns, tagOneSeriesDefn, tagValueTypeID, tagQuantityUnitsID,
  tagQuantityCharacteristicID, tagStorageMethodID, tagSeriesNominalQuantity,
  tagHintGreekPrefixID, tagHintPreferredUnitsID, tagHintDefaultDisplayID,
  tagQuantitySignificantDigitsID, tagQuantityResolutionID,
  tagEffective,
} = require('../constants/tagGuids');

/**
 * CPQDIF_R_DataSource - data source record.
 * Ported from rec_datasource.h/rec_datasource.cpp
 */
class DataSourceRecord extends Record {
  constructor() {
    super();
  }

  // ============================================================
  // Read functions
  // ============================================================

  /**
   * Get data source info.
   * @param {Object} out - { idDataSourceType, idVendor, idEquipment, serialNumber, version, name, owner, location, timeZone }
   * @returns {boolean}
   */
  getInfo(out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    // Find GUIDs
    let psc = pcollMain.getScalar(tagDataSourceTypeID);
    if (psc) {
      const r = psc.getValueGUID();
      if (r.status) out.idDataSourceType = r.value;
    }

    psc = pcollMain.getScalar(tagVendorID);
    if (psc) {
      const r = psc.getValueGUID();
      if (r.status) out.idVendor = r.value;
    }

    psc = pcollMain.getScalar(tagEquipmentID);
    if (psc) {
      const r = psc.getValueGUID();
      if (r.status) out.idEquipment = r.value;
    }

    // Find strings
    let pvect = pcollMain.getVector(tagSerialNumberDS);
    if (pvect) out.serialNumber = pvect.getString();

    pvect = pcollMain.getVector(tagVersionDS);
    if (pvect) out.version = pvect.getString();

    pvect = pcollMain.getVector(tagNameDS);
    if (pvect) out.name = pvect.getString();

    pvect = pcollMain.getVector(tagOwnerDS);
    if (pvect) out.owner = pvect.getString();

    pvect = pcollMain.getVector(tagLocationDS);
    if (pvect) out.location = pvect.getString();

    pvect = pcollMain.getVector(tagTimeZoneDS);
    if (pvect) out.timeZone = pvect.getString();

    return true;
  }

  /**
   * Get count of channel definitions.
   * @returns {number}
   */
  getCountChannelDefns() {
    const pcollDefns = this.getChannelDefns();
    if (pcollDefns) {
      return pcollDefns.getCount();
    }
    return 0;
  }

  /**
   * Get count of series definitions for a channel.
   * @param {number} idxChannelDefn
   * @returns {number}
   */
  getCountSeriesDefns(idxChannelDefn) {
    const pcolOneChannelDefn = this.getOneChannelDefn(idxChannelDefn);
    if (pcolOneChannelDefn) {
      const pcollSeriesDefns = this.getSeriesDefns(pcolOneChannelDefn);
      if (pcollSeriesDefns) {
        return pcollSeriesDefns.getCount();
      }
    }
    return 0;
  }

  /**
   * Get channel definition info.
   * @param {number} idxChannelDefn
   * @param {Object} out - { name, idPhase, idQuantityType, idQuantityMeasured }
   * @returns {boolean}
   */
  getChannelDefnInfo(idxChannelDefn, out) {
    const pcolOneChannelDefn = this.getOneChannelDefn(idxChannelDefn);
    if (!pcolOneChannelDefn) return false;

    // Find channel name (optional)
    const pvecName = Record.findVectorInCollection(pcolOneChannelDefn, tagChannelName);
    if (pvecName) {
      out.name = pvecName.getString();
    }

    // Find phase ID (required)
    const pscPhaseID = Record.findScalarInCollection(pcolOneChannelDefn, tagPhaseID);
    let status = false;
    if (pscPhaseID) {
      const r = pscPhaseID.getValueUINT4();
      if (r.status) {
        out.idPhase = r.value;
        status = true;
      }
    }

    // Find quantity type ID (required)
    if (status) {
      const pscQuantityTypeID = Record.findScalarInCollection(pcolOneChannelDefn, tagQuantityTypeID);
      if (pscQuantityTypeID) {
        const r = pscQuantityTypeID.getValueGUID();
        if (r.status) {
          out.idQuantityType = r.value;
        } else {
          status = false;
        }
      }
    }

    // Find quantity measured ID (required)
    if (status) {
      const pscQuantityMeasuredID = Record.findScalarInCollection(pcolOneChannelDefn, tagQuantityMeasuredID);
      if (pscQuantityMeasuredID) {
        const r = pscQuantityMeasuredID.getValueUINT4();
        if (r.status) {
          out.idQuantityMeasured = r.value;
        } else {
          status = false;
        }
      }
    }

    return status;
  }

  /**
   * Get primary series index for a channel definition.
   * @param {number} idxChannelDefn
   * @param {Object} out - { idxPrimarySeries }
   * @returns {boolean}
   */
  getChannelPrimarySeries(idxChannelDefn, out) {
    const pcolOneChannelDefn = this.getOneChannelDefn(idxChannelDefn);
    if (!pcolOneChannelDefn) return false;

    const psc = Record.findScalarInCollection(pcolOneChannelDefn, tagPrimarySeriesIdx);
    if (psc) {
      const r = psc.getValueUINT4();
      if (r.status) {
        out.idxPrimarySeries = r.value;
        return true;
      }
    }
    return false;
  }

  /**
   * Get series definition info.
   * @param {number} idxChannelDefn
   * @param {number} idxSeriesDefn
   * @param {Object} out - { idQuantityUnits, idValueType, idQuantityCharacteristic, idStorageMethod }
   * @returns {boolean}
   */
  getSeriesDefnInfo(idxChannelDefn, idxSeriesDefn, out) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;

    let status = true;

    // Quantity units (required)
    let pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagQuantityUnitsID);
    if (status && pscalar) {
      const r = pscalar.getValueUINT4();
      if (r.status) out.idQuantityUnits = r.value;
      else status = false;
    }

    // Value type (required)
    pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagValueTypeID);
    if (status && pscalar) {
      const r = pscalar.getValueGUID();
      if (r.status) out.idValueType = r.value;
      else status = false;
    }

    // Quantity characteristic (required)
    pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagQuantityCharacteristicID);
    if (status && pscalar) {
      const r = pscalar.getValueGUID();
      if (r.status) out.idQuantityCharacteristic = r.value;
      else status = false;
    }

    // Storage method (required)
    pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagStorageMethodID);
    if (status && pscalar) {
      const r = pscalar.getValueUINT4();
      if (r.status) out.idStorageMethod = r.value;
      else status = false;
    }

    return status;
  }

  // ============================================================
  // Write functions
  // ============================================================

  /**
   * Add a channel definition.
   * @param {string} name
   * @param {number} idPhase
   * @param {Buffer} idQuantityType - GUID
   * @returns {number} index of new channel definition, or -1
   */
  addChannelDefn(name, idPhase, idQuantityType) {
    const pcollDefns = this.getChannelDefns();
    if (!pcollDefns) return -1;

    const idxNew = this.getCountChannelDefns();

    // Create the new channel definition collection
    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneChannelDefn);
      pcollOne.setVectorString(tagChannelName, name);
      pcollOne.setScalarUINT4(tagPhaseID, idPhase);
      pcollOne.setScalarGUID(tagQuantityTypeID, idQuantityType);

      // Add empty series definitions collection
      const pcollSeriesDefns = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcollSeriesDefns) {
        pcollSeriesDefns.setTag(tagSeriesDefns);
        pcollOne.add(pcollSeriesDefns);
      }

      pcollDefns.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Add a channel definition with quantity measured ID.
   * @param {string} name
   * @param {number} idPhase
   * @param {number} idQM
   * @param {Buffer} idQuantityType - GUID
   * @returns {number} index of new channel definition, or -1
   */
  addChannelDefn2(name, idPhase, idQM, idQuantityType) {
    const pcollDefns = this.getChannelDefns();
    if (!pcollDefns) return -1;

    const idxNew = this.getCountChannelDefns();

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneChannelDefn);
      pcollOne.setVectorString(tagChannelName, name);
      pcollOne.setScalarUINT4(tagPhaseID, idPhase);
      pcollOne.setScalarUINT4(tagQuantityMeasuredID, idQM);
      pcollOne.setScalarGUID(tagQuantityTypeID, idQuantityType);

      const pcollSeriesDefns = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcollSeriesDefns) {
        pcollSeriesDefns.setTag(tagSeriesDefns);
        pcollOne.add(pcollSeriesDefns);
      }

      pcollDefns.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Add a series definition.
   * @param {number} idxChannelDefn
   * @param {number} idQuantityUnits
   * @param {Buffer} idValueType - GUID
   * @param {number} idStorageMethod
   * @returns {number} index of new series definition, or -1
   */
  addSeriesDefn(idxChannelDefn, idQuantityUnits, idValueType, idStorageMethod) {
    const pcolOneChannelDefn = this.getOneChannelDefn(idxChannelDefn);
    if (!pcolOneChannelDefn) return -1;

    const pcollSeriesDefns = this.getSeriesDefns(pcolOneChannelDefn);
    if (!pcollSeriesDefns) return -1;

    const idxNew = this.getCountSeriesDefns(idxChannelDefn);

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneSeriesDefn);
      pcollOne.setScalarUINT4(tagQuantityUnitsID, idQuantityUnits);
      pcollOne.setScalarGUID(tagValueTypeID, idValueType);
      pcollOne.setScalarUINT4(tagStorageMethodID, idStorageMethod);
      pcollSeriesDefns.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Add a series definition with characteristic type.
   * @param {number} idxChannelDefn
   * @param {number} idQuantityUnits
   * @param {Buffer} idValueType - GUID
   * @param {Buffer} idCharacteristicType - GUID
   * @param {number} idStorageMethod
   * @returns {number} index of new series definition, or -1
   */
  addSeriesDefn2(idxChannelDefn, idQuantityUnits, idValueType, idCharacteristicType, idStorageMethod) {
    const pcolOneChannelDefn = this.getOneChannelDefn(idxChannelDefn);
    if (!pcolOneChannelDefn) return -1;

    const pcollSeriesDefns = this.getSeriesDefns(pcolOneChannelDefn);
    if (!pcollSeriesDefns) return -1;

    const idxNew = this.getCountSeriesDefns(idxChannelDefn);

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneSeriesDefn);
      pcollOne.setScalarUINT4(tagQuantityUnitsID, idQuantityUnits);
      pcollOne.setScalarGUID(tagQuantityCharacteristicID, idCharacteristicType);
      pcollOne.setScalarGUID(tagValueTypeID, idValueType);
      pcollOne.setScalarUINT4(tagStorageMethodID, idStorageMethod);
      pcollSeriesDefns.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Get nominal value for a series definition.
   * @param {number} idxChannelDefn
   * @param {number} idxSeriesDefn
   * @param {Object} out - { dNominal }
   * @returns {boolean}
   */
  getSeriesDefnNominal(idxChannelDefn, idxSeriesDefn, out) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;

    const pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagSeriesNominalQuantity);
    if (pscalar) {
      const r = pscalar.getValueREAL8();
      if (r.status) {
        out.dNominal = r.value;
        return true;
      }
    }
    return false;
  }

  /**
   * Set nominal value for a series definition.
   * @param {number} idxChannelDefn
   * @param {number} idxSeriesDefn
   * @param {number} dNominal
   * @returns {boolean}
   */
  setSeriesDefnNominal(idxChannelDefn, idxSeriesDefn, dNominal) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;

    let pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagSeriesNominalQuantity);
    if (pscalar) {
      return pscalar.setValueREAL8(dNominal);
    } else {
      const factory = require('./factory');
      pscalar = factory.newElement(2);
      if (pscalar) {
        pscalar.setTag(tagSeriesNominalQuantity);
        pscalar.setValueREAL8(dNominal);
        pcolOneSeriesDefn.add(pscalar);
        return true;
      }
    }
    return false;
  }

  /**
   * Get precision and resolution for a series definition.
   * @param {number} idxChannel
   * @param {number} idxSeries
   * @param {Object} out - { uPrecision, dResolution }
   * @returns {boolean}
   */
  getSeriesDefnPrecisionAndResolution(idxChannel, idxSeries, out) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannel, idxSeries);
    if (!pcolOneSeriesDefn) return false;

    let status = false;

    const pscalar = Record.findScalarInCollection(pcolOneSeriesDefn, tagQuantitySignificantDigitsID);
    if (pscalar) {
      const r = pscalar.getValueUINT4();
      if (r.status) {
        out.uPrecision = r.value;
        status = true;
      }
    }

    const pscalar2 = Record.findScalarInCollection(pcolOneSeriesDefn, tagQuantityResolutionID);
    if (status && pscalar2) {
      const r = pscalar2.getValueREAL8();
      if (r.status) {
        out.dResolution = r.value;
      } else {
        status = false;
      }
    }

    return status;
  }

  /**
   * Set series definition prefix hint.
   */
  setSeriesDefnPrefix(idxChannelDefn, idxSeriesDefn, idPrefix) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;
    pcolOneSeriesDefn.setScalarUINT4(tagHintGreekPrefixID, idPrefix, true);
    return true;
  }

  /**
   * Set series definition display hint.
   */
  setSeriesDefnDisplay(idxChannelDefn, idxSeriesDefn, idDisplay) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;
    pcolOneSeriesDefn.setScalarUINT4(tagHintDefaultDisplayID, idDisplay, true);
    return true;
  }

  /**
   * Set series definition units hint.
   */
  setSeriesDefnUnits(idxChannelDefn, idxSeriesDefn, idUnits) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;
    pcolOneSeriesDefn.setScalarUINT4(tagHintPreferredUnitsID, idUnits, true);
    return true;
  }

  /**
   * Set series definition resolution.
   */
  setSeriesDefnResolution(idxChannelDefn, idxSeriesDefn, dRes) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;
    pcolOneSeriesDefn.setScalarREAL8(tagQuantityResolutionID, dRes, true);
    return true;
  }

  /**
   * Set series definition digits.
   */
  setSeriesDefnDigits(idxChannelDefn, idxSeriesDefn, idDigits) {
    const pcolOneSeriesDefn = this.getOneSeriesDefn(idxChannelDefn, idxSeriesDefn);
    if (!pcolOneSeriesDefn) return false;
    pcolOneSeriesDefn.setScalarUINT4(tagQuantitySignificantDigitsID, idDigits, true);
    return true;
  }

  // ============================================================
  // Effective time (delegated to main collection)
  // ============================================================

  setEffective(timeEffective) {
    return this.setTimeInMainCollection(tagEffective, timeEffective);
  }

  getEffective(outTime) {
    return this.getTimeInMainCollection(tagEffective, outTime);
  }

  // ============================================================
  // Internal helper functions
  // ============================================================

  /**
   * Get the channel definitions collection (create if not exists).
   * @returns {Collection|null}
   */
  getChannelDefns() {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return null;

    let pcollDefns = Record.findCollectionInCollection(pcollMain, tagChannelDefns);
    if (!pcollDefns) {
      const factory = require('./factory');
      pcollDefns = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcollDefns) {
        pcollDefns.setTag(tagChannelDefns);
        pcollMain.add(pcollDefns);
      }
    }

    return pcollDefns;
  }

  /**
   * Get one channel definition by index.
   * @param {number} idxChannelDefn
   * @returns {Collection|null}
   */
  getOneChannelDefn(idxChannelDefn) {
    const pcolInstances = this.getChannelDefns();
    if (!pcolInstances) return null;

    const pel = pcolInstances.getElement(idxChannelDefn);
    if (pel &&
        pel.getElementType() === ID_ELEMENT_TYPE_COLLECTION &&
        pel.getTag() && guidEquals(pel.getTag(), tagOneChannelDefn)) {
      return pel;
    }

    return null;
  }

  /**
   * Get series definitions collection for a channel definition.
   * @param {Collection} pcolOneChannelDefn
   * @returns {Collection|null}
   */
  getSeriesDefns(pcolOneChannelDefn) {
    return Record.findCollectionInCollection(pcolOneChannelDefn, tagSeriesDefns);
  }

  /**
   * Get one series definition by index.
   * @param {Collection} pcolOneChannelDefn
   * @param {number} idxSeriesDefn
   * @returns {Collection|null}
   */
  getOneSeriesDefnByChannel(pcolOneChannelDefn, idxSeriesDefn) {
    const pcolSeriesDefns = this.getSeriesDefns(pcolOneChannelDefn);
    if (!pcolSeriesDefns) return null;

    const pel = pcolSeriesDefns.getElement(idxSeriesDefn);
    if (pel &&
        pel.getElementType() === ID_ELEMENT_TYPE_COLLECTION &&
        pel.getTag() && guidEquals(pel.getTag(), tagOneSeriesDefn)) {
      return pel;
    }

    return null;
  }

  /**
   * Get one series definition by channel and series indices.
   * @param {number} idxChannelDefn
   * @param {number} idxSeriesDefn
   * @returns {Collection|null}
   */
  getOneSeriesDefn(idxChannelDefn, idxSeriesDefn) {
    const pcolOneChannel = this.getOneChannelDefn(idxChannelDefn);
    if (!pcolOneChannel) return null;
    return this.getOneSeriesDefnByChannel(pcolOneChannel, idxSeriesDefn);
  }
}

module.exports = { DataSourceRecord };
