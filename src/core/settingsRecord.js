'use strict';

const { Record } = require('./record');
const { Collection } = require('./collection');
const { PqdifInfo, guidEquals } = require('../info/info');
const {
  ID_PHYS_TYPE_UNS_INTEGER4, ID_PHYS_TYPE_REAL8, ID_PHYS_TYPE_BOOLEAN4,
  ID_PHYS_TYPE_TIMESTAMPPQDIF,
} = require('../constants/physicalTypes');
const { ID_ELEMENT_TYPE_COLLECTION } = require('../constants/elementTypes');
const {
  tagEffective, tagTimeInstalled, tagTimeRemoved,
  tagUseCalibration, tagUseTransducer,
  tagChannelSettingsArray, tagOneChannelSetting, tagChannelDefnIdx,
  tagTriggerTypeID, tagFullScale, tagNoiseFloor,
  tagTriggerLow, tagTriggerHigh, tagTriggerRate, tagTriggerShapeParam,
  tagXDTransformerTypeID, tagXDSystemSideRatio, tagXDMonitorSideRatio, tagXDFrequencyResponse,
  tagCalTimeSkew, tagCalOffset, tagCalRatio, tagCalMustUseARCal,
  tagCalApplied, tagCalRecorded,
  tagSettingPhysicalConnection, tagNominalFrequency,
} = require('../constants/tagGuids');

/**
 * CPQDIF_R_Settings - monitor settings record.
 * Ported from rec_settings.h/rec_settings.cpp
 */
class SettingsRecord extends Record {
  constructor() {
    super();
  }

  // ============================================================
  // Read functions
  // ============================================================

  /**
   * Get settings info.
   * @param {Object} out - { timeEffective, timeInstalled, timeRemoved, useCal, useTrans }
   * @returns {boolean}
   */
  getInfo(out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    let status = true;

    // Effective time (required)
    const pscEff = Record.findScalarInCollection(pcollMain, tagEffective);
    if (pscEff) {
      const r = pscEff.getValueTimeStamp();
      if (r.status) out.timeEffective = r.value;
      else status = false;
    } else {
      status = false;
    }

    // Installed time (optional)
    if (status) {
      const pscInst = Record.findScalarInCollection(pcollMain, tagTimeInstalled);
      if (pscInst) {
        const r = pscInst.getValueTimeStamp();
        if (r.status) out.timeInstalled = r.value;
      }
    }

    // Removed time (optional)
    if (status) {
      const pscRem = Record.findScalarInCollection(pcollMain, tagTimeRemoved);
      if (pscRem) {
        const r = pscRem.getValueTimeStamp();
        if (r.status) out.timeRemoved = r.value;
      }
    }

    // UseCalibration (required)
    if (status) {
      const pscCal = Record.findScalarInCollection(pcollMain, tagUseCalibration);
      if (pscCal) {
        const r = pscCal.getValueBOOL4();
        if (r.status) out.useCal = r.value;
        else status = false;
      } else {
        status = false;
      }
    }

    // UseTransducer (required)
    if (status) {
      const pscTrans = Record.findScalarInCollection(pcollMain, tagUseTransducer);
      if (pscTrans) {
        const r = pscTrans.getValueBOOL4();
        if (r.status) out.useTrans = r.value;
        else status = false;
      } else {
        status = false;
      }
    }

    return status;
  }

  /**
   * Get connection info.
   * @param {Object} out - { connectionType }
   * @returns {boolean}
   */
  getConnectionInfo(out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    const psc = Record.findScalarInCollection(pcollMain, tagSettingPhysicalConnection);
    if (psc) {
      const r = psc.getValueUINT4();
      if (r.status) {
        out.connectionType = r.value;
        return true;
      }
    }
    return false;
  }

  /**
   * Get count of channel settings.
   * @returns {number}
   */
  getCountChannels() {
    const pcolCI = this.getChannelSettings();
    if (pcolCI) {
      return pcolCI.getCount();
    }
    return 0;
  }

  /**
   * Get channel info by index.
   * @param {number} idxChannel
   * @param {Object} out - { idxChannelDefn }
   * @returns {boolean}
   */
  getChannelInfo(idxChannel, out) {
    const pcollMain = this.getOneChannelSetting(idxChannel);
    if (!pcollMain) return false;

    const outVal = { value: 0 };
    const found = Record.getScalarValueInCollection(pcollMain, tagChannelDefnIdx, ID_PHYS_TYPE_UNS_INTEGER4, outVal);
    if (found) {
      out.idxChannelDefn = outVal.value;
    }
    return found;
  }

  /**
   * Get transducer info for a channel.
   * @param {number} idxChannel
   * @param {Object} out - { xdTransformerTypeID, xdSystemSideRatio, xdMonitorSideRatio, xdFrequencyResponse }
   * @returns {boolean}
   */
  getChanTrans(idxChannel, out) {
    const pcollMain = this.getOneChannelSetting(idxChannel);
    if (!pcollMain) return false;

    let status = true;

    // xdTransformerTypeID (required)
    const outVal = { value: 0 };
    if (Record.getScalarValueInCollection(pcollMain, tagXDTransformerTypeID, ID_PHYS_TYPE_UNS_INTEGER4, outVal)) {
      out.xdTransformerTypeID = outVal.value;
    } else {
      status = false;
    }

    // xdSystemSideRatio (required)
    const outReal = { value: 0.0 };
    if (status && Record.getScalarValueInCollection(pcollMain, tagXDSystemSideRatio, ID_PHYS_TYPE_REAL8, outReal)) {
      out.xdSystemSideRatio = outReal.value;
    } else {
      status = false;
    }

    // xdMonitorSideRatio (required)
    if (status && Record.getScalarValueInCollection(pcollMain, tagXDMonitorSideRatio, ID_PHYS_TYPE_REAL8, outReal)) {
      out.xdMonitorSideRatio = outReal.value;
    } else {
      status = false;
    }

    // xdFrequencyResponse (required)
    if (status) {
      const pvect = Record.findVectorInCollection(pcollMain, tagXDFrequencyResponse);
      if (pvect && pvect.getPhysicalType() === ID_PHYS_TYPE_REAL8) {
        const count = pvect.getCount();
        const values = [];
        for (let idx = 0; idx < count; idx++) {
          const r = pvect.GetValueREAL8(idx, outReal);
          values.push(outReal.value);
        }
        out.xdFrequencyResponse = values;
      } else {
        status = false;
      }
    }

    return status;
  }

  /**
   * Get calibration info for a channel.
   * @param {number} idxChannel
   * @param {Object} out - { calTimeSkew, calOffset, calRatio, calMustUseARCal, calApplied, calRecorded }
   * @returns {boolean}
   */
  getChanCal(idxChannel, out) {
    const pcollMain = this.getOneChannelSetting(idxChannel);
    if (!pcollMain) return false;

    let status = true;
    const outReal = { value: 0.0 };

    // calTimeSkew (required)
    if (Record.getScalarValueInCollection(pcollMain, tagCalTimeSkew, ID_PHYS_TYPE_REAL8, outReal)) {
      out.calTimeSkew = outReal.value;
    } else {
      status = false;
    }

    // calOffset (required)
    if (status && Record.getScalarValueInCollection(pcollMain, tagCalOffset, ID_PHYS_TYPE_REAL8, outReal)) {
      out.calOffset = outReal.value;
    } else {
      status = false;
    }

    // calRatio (required)
    if (status && Record.getScalarValueInCollection(pcollMain, tagCalRatio, ID_PHYS_TYPE_REAL8, outReal)) {
      out.calRatio = outReal.value;
    } else {
      status = false;
    }

    // calMustUseARCal (required)
    const outBool = { value: false };
    if (status && Record.getScalarValueInCollection(pcollMain, tagCalMustUseARCal, ID_PHYS_TYPE_BOOLEAN4, outBool)) {
      out.calMustUseARCal = outBool.value;
    } else {
      status = false;
    }

    // calApplied (required)
    if (status) {
      const pvect = Record.findVectorInCollection(pcollMain, tagCalApplied);
      if (pvect && pvect.getPhysicalType() === ID_PHYS_TYPE_REAL8) {
        const count = pvect.getCount();
        const values = [];
        for (let idx = 0; idx < count; idx++) {
          pvect.GetValueREAL8(idx, outReal);
          values.push(outReal.value);
        }
        out.calApplied = values;
      } else {
        status = false;
      }
    }

    // calRecorded (required)
    if (status) {
      const pvect = Record.findVectorInCollection(pcollMain, tagCalRecorded);
      if (pvect && pvect.getPhysicalType() === ID_PHYS_TYPE_REAL8) {
        const count = pvect.getCount();
        const values = [];
        for (let idx = 0; idx < count; idx++) {
          pvect.GetValueREAL8(idx, outReal);
          values.push(outReal.value);
        }
        out.calRecorded = values;
      } else {
        status = false;
      }
    }

    return status;
  }

  // ============================================================
  // Write functions
  // ============================================================

  /**
   * Set settings info.
   */
  setInfo(timeEffective, timeInstalled, timeRemoved, useCal, useTrans) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    let status = true;
    const factory = require('./factory');

    // Effective time
    let psc = Record.findOrCreateScalarInCollection(pcollMain, tagEffective, ID_PHYS_TYPE_TIMESTAMPPQDIF);
    if (psc) psc.setValueTimeStamp(timeEffective);
    else status = false;

    // Installed time
    if (status) {
      psc = Record.findOrCreateScalarInCollection(pcollMain, tagTimeInstalled, ID_PHYS_TYPE_TIMESTAMPPQDIF);
      if (psc) psc.setValueTimeStamp(timeInstalled);
      else status = false;
    }

    // Removed time (optional - only set if non-null)
    if (status && timeRemoved !== null && timeRemoved !== undefined) {
      psc = Record.findOrCreateScalarInCollection(pcollMain, tagTimeRemoved, ID_PHYS_TYPE_TIMESTAMPPQDIF);
      if (psc) psc.setValueTimeStamp(timeRemoved);
      else status = false;
    }

    // UseCalibration
    if (status) {
      psc = Record.findOrCreateScalarInCollection(pcollMain, tagUseCalibration, ID_PHYS_TYPE_BOOLEAN4);
      if (psc) psc.setValueBOOL4(useCal);
      else status = false;
    }

    // UseTransducer
    if (status) {
      psc = Record.findOrCreateScalarInCollection(pcollMain, tagUseTransducer, ID_PHYS_TYPE_BOOLEAN4);
      if (psc) psc.setValueBOOL4(useTrans);
      else status = false;
    }

    // Ensure channel settings collection exists
    let pcolInstances = this.getChannelSettings();
    if (!pcolInstances) {
      pcolInstances = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcolInstances) {
        pcolInstances.setTag(tagChannelSettingsArray);
        pcollMain.add(pcolInstances);
      }
    }

    return status;
  }

  /**
   * Set connection info.
   */
  setConnectionInfo(connectionType) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    const psc = Record.findOrCreateScalarInCollection(pcollMain, tagSettingPhysicalConnection, ID_PHYS_TYPE_UNS_INTEGER4);
    if (psc) {
      return psc.setValueUINT4(connectionType);
    }
    return false;
  }

  /**
   * Add a channel setting.
   * @param {number} idxChannelDefn
   * @returns {number} index of new channel setting
   */
  addChannel(idxChannelDefn) {
    let pcolInstances = this.getChannelSettings();
    if (!pcolInstances) {
      const factory = require('./factory');
      pcolInstances = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcolInstances) {
        pcolInstances.setTag(tagChannelSettingsArray);
        this.getMainCollection().add(pcolInstances);
      }
    }

    if (!pcolInstances) return -1;

    const idxNew = this.getCountChannels();

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneChannelSetting);
      pcollOne.setScalarUINT4(tagChannelDefnIdx, idxChannelDefn);
      pcolInstances.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Add a channel setting with trigger type.
   */
  addChannelWithTrigger(idxChannelDefn, idTriggerType) {
    let pcolInstances = this.getChannelSettings();
    if (!pcolInstances) {
      const factory = require('./factory');
      pcolInstances = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcolInstances) {
        pcolInstances.setTag(tagChannelSettingsArray);
        this.getMainCollection().add(pcolInstances);
      }
    }

    if (!pcolInstances) return -1;

    const idxNew = this.getCountChannels();

    const factory = require('./factory');
    const pcollOne = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
    if (pcollOne) {
      pcollOne.setTag(tagOneChannelSetting);
      pcollOne.setScalarUINT4(tagChannelDefnIdx, idxChannelDefn);
      pcollOne.setScalarUINT4(tagTriggerTypeID, idTriggerType);
      pcolInstances.add(pcollOne);
    }

    return idxNew;
  }

  /**
   * Set transducer info for a channel.
   */
  setChanTrans(idxChannel, xdTransformerTypeID, xdSystemSideRatio, xdMonitorSideRatio, xdFrequencyResponse) {
    const pcollOne = this.getOneChannelSetting(idxChannel);
    if (!pcollOne) return false;

    pcollOne.setScalarUINT4(tagXDTransformerTypeID, xdTransformerTypeID, true);
    pcollOne.setScalarREAL8(tagXDSystemSideRatio, xdSystemSideRatio, true);
    pcollOne.setScalarREAL8(tagXDMonitorSideRatio, xdMonitorSideRatio, true);

    if (xdFrequencyResponse && Array.isArray(xdFrequencyResponse)) {
      pcollOne.setVectorREAL8(tagXDFrequencyResponse, xdFrequencyResponse, xdFrequencyResponse.length, true);
    }

    return true;
  }

  /**
   * Set calibration info for a channel.
   */
  setChanCal(idxChannel, calTimeSkew, calOffset, calRatio, calMustUseARCal, calApplied, calRecorded) {
    const pcollOne = this.getOneChannelSetting(idxChannel);
    if (!pcollOne) return false;

    pcollOne.setScalarREAL8(tagCalTimeSkew, calTimeSkew, true);
    pcollOne.setScalarREAL8(tagCalOffset, calOffset, true);
    pcollOne.setScalarREAL8(tagCalRatio, calRatio, true);
    pcollOne.setScalarBOOL4(tagCalMustUseARCal, calMustUseARCal, true);

    if (calApplied && Array.isArray(calApplied)) {
      pcollOne.setVectorREAL8(tagCalApplied, calApplied, calApplied.length, true);
    }
    if (calRecorded && Array.isArray(calRecorded)) {
      pcollOne.setVectorREAL8(tagCalRecorded, calRecorded, calRecorded.length, true);
    }

    return true;
  }

  // ============================================================
  // Convenience inline methods
  // ============================================================

  setEffective(timeEffective) { return this.setTimeInMainCollection(tagEffective, timeEffective); }
  getEffective(outTime) { return this.getTimeInMainCollection(tagEffective, outTime); }

  setInstalled(timeInstalled) { return this.setTimeInMainCollection(tagTimeInstalled, timeInstalled); }
  getInstalled(outTime) { return this.getTimeInMainCollection(tagTimeInstalled, outTime); }

  setRemoved(timeRemoved) { return this.setTimeInMainCollection(tagTimeRemoved, timeRemoved); }
  getRemoved(outTime) { return this.getTimeInMainCollection(tagTimeRemoved, outTime); }

  setNominalFrequency(dVal) { return this.setREAL8InMainCollection(tagNominalFrequency, dVal); }
  getNominalFrequency(outVal) { return this.getREAL8InMainCollection(tagNominalFrequency, outVal); }

  setUseCalibration(bVal) { return this.setBOOL4InMainCollection(tagUseCalibration, bVal); }
  getUseCalibration(outVal) { return this.getBOOL4InMainCollection(tagUseCalibration, outVal); }

  setUseTransducer(bVal) { return this.setBOOL4InMainCollection(tagUseTransducer, bVal); }
  getUseTransducer(outVal) { return this.getBOOL4InMainCollection(tagUseTransducer, outVal); }

  // ============================================================
  // Channel-specific real8 getters/setters
  // ============================================================

  setChannelReal8(idxChannel, tag, dVal) {
    const pcollOne = this.getOneChannelSetting(idxChannel);
    if (!pcollOne) return false;
    const psc = Record.findOrCreateScalarInCollection(pcollOne, tag, ID_PHYS_TYPE_REAL8);
    if (psc) {
      return psc.setValueREAL8(dVal);
    }
    return false;
  }

  getChannelReal8(idxChannel, tag, out) {
    const pcollOne = this.getOneChannelSetting(idxChannel);
    if (!pcollOne) return false;
    const outVal = { value: 0.0 };
    const found = Record.getScalarValueInCollection(pcollOne, tag, ID_PHYS_TYPE_REAL8, outVal);
    if (found) out.value = outVal.value;
    return found;
  }

  setTriggerLow(idxChannel, dVal) { return this.setChannelReal8(idxChannel, tagTriggerLow, dVal); }
  getTriggerLow(idxChannel, out) { return this.getChannelReal8(idxChannel, tagTriggerLow, out); }

  setTriggerHigh(idxChannel, dVal) { return this.setChannelReal8(idxChannel, tagTriggerHigh, dVal); }
  getTriggerHigh(idxChannel, out) { return this.getChannelReal8(idxChannel, tagTriggerHigh, out); }

  setTriggerRate(idxChannel, dVal) { return this.setChannelReal8(idxChannel, tagTriggerRate, dVal); }
  getTriggerRate(idxChannel, out) { return this.getChannelReal8(idxChannel, tagTriggerRate, out); }

  setFullScale(idxChannel, dVal) { return this.setChannelReal8(idxChannel, tagFullScale, dVal); }
  getFullScale(idxChannel, out) { return this.getChannelReal8(idxChannel, tagFullScale, out); }

  setNoiseFloor(idxChannel, dVal) { return this.setChannelReal8(idxChannel, tagNoiseFloor, dVal); }
  getNoiseFloor(idxChannel, out) { return this.getChannelReal8(idxChannel, tagNoiseFloor, out); }

  // ============================================================
  // Internal functions
  // ============================================================

  /**
   * Get the channel settings collection (create if not exists).
   * @returns {Collection|null}
   */
  getChannelSettings() {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return null;

    let pcollSettings = Record.findCollectionInCollection(pcollMain, tagChannelSettingsArray);
    if (!pcollSettings) {
      const factory = require('./factory');
      pcollSettings = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
      if (pcollSettings) {
        pcollSettings.setTag(tagChannelSettingsArray);
        pcollMain.add(pcollSettings);
      }
    }

    return pcollSettings;
  }

  /**
   * Get one channel setting by index.
   * @param {number} idxChannel
   * @returns {Collection|null}
   */
  getOneChannelSetting(idxChannel) {
    const pcolInstances = this.getChannelSettings();
    if (!pcolInstances) return null;

    const pel = pcolInstances.getElement(idxChannel);
    if (pel &&
        pel.getElementType() === ID_ELEMENT_TYPE_COLLECTION &&
        pel.getTag() && guidEquals(pel.getTag(), tagOneChannelSetting)) {
      return pel;
    }

    return null;
  }
}

module.exports = { SettingsRecord };
