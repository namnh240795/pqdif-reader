'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement } = require('./utilities');

/**
 * Channel setting - ported from OneChannelSetting.cs.
 */
class ChannelSetting {
  constructor() {
    /** @type {number} Channel definition index */
    this.channelDefnIdx = 0;

    /** @type {number|null} Trigger type ID */
    this.triggerTypeID = null;

    /** @type {number|null} Full scale range */
    this.fullScale = null;

    /** @type {number|null} Noise floor */
    this.noiseFloor = null;

    /** @type {number[]|null} Trigger shape parameters */
    this.triggerShapeParam = null;

    /** @type {number|null} Transformer type ID */
    this.xdTransformerTypeID = null;

    /** @type {number|null} System side ratio */
    this.xdSystemSideRatio = null;

    /** @type {number|null} Monitor side ratio */
    this.xdMonitorSideRatio = null;

    /** @type {number[]|null} Frequency response */
    this.xdFrequencyResponse = null;

    /** @type {number|null} Time skew */
    this.calTimeSkew = null;

    /** @type {number|null} DC offset error */
    this.calOffset = null;

    /** @type {number|null} Ratio error */
    this.calRatio = null;

    /** @type {number|null} Must use AR calibration */
    this.calMustUseARCal = null;

    /** @type {number[]|null} Applied signals */
    this.calApplied = null;

    /** @type {number[]|null} Recorded actual values */
    this.calRecorded = null;

    /** @type {number|null} High-high trigger */
    this.triggerHighHigh = null;

    /** @type {number|null} High trigger */
    this.triggerHigh = null;

    /** @type {number|null} Low trigger */
    this.triggerLow = null;

    /** @type {number|null} Low-low trigger */
    this.triggerLowLow = null;

    /** @type {number|null} Deadband trigger */
    this.triggerDeadBand = null;

    /** @type {number|null} Rate of change trigger */
    this.triggerRate = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {ChannelSetting}
   */
  static fromCollection(coll) {
    const cs = new ChannelSetting();
    if (!coll) return cs;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagChannelDefnIdx)) {
          cs.channelDefnIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerTypeID)) {
          cs.triggerTypeID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagFullScale)) {
          cs.fullScale = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagNoiseFloor)) {
          cs.noiseFloor = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerShapeParam)) {
          cs.triggerShapeParam = _getDoubleArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagXDTransformerTypeID)) {
          cs.xdTransformerTypeID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagXDSystemSideRatio)) {
          cs.xdSystemSideRatio = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagXDMonitorSideRatio)) {
          cs.xdMonitorSideRatio = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagXDFrequencyResponse)) {
          cs.xdFrequencyResponse = _getDoubleArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCalTimeSkew)) {
          cs.calTimeSkew = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCalOffset)) {
          cs.calOffset = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCalRatio)) {
          cs.calRatio = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCalMustUseARCal)) {
          cs.calMustUseARCal = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCalApplied)) {
          cs.calApplied = _getDoubleArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCalRecorded)) {
          cs.calRecorded = _getDoubleArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerHighHigh)) {
          cs.triggerHighHigh = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerHigh)) {
          cs.triggerHigh = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerLow)) {
          cs.triggerLow = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerLowLow)) {
          cs.triggerLowLow = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerDeadBand)) {
          cs.triggerDeadBand = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerRate)) {
          cs.triggerRate = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          cs.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        cs.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    return cs;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagOneChannelSetting>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagChannelDefnIdx, this.channelDefnIdx) + '\r\n';
    if (this.triggerTypeID !== null) xml += xmlGetElement(tagGuids.tagTriggerTypeID, this.triggerTypeID) + '\r\n';
    if (this.fullScale !== null) xml += xmlGetElement(tagGuids.tagFullScale, this.fullScale) + '\r\n';
    if (this.noiseFloor !== null) xml += xmlGetElement(tagGuids.tagNoiseFloor, this.noiseFloor) + '\r\n';
    if (this.triggerShapeParam !== null) xml += xmlGetElement(tagGuids.tagTriggerShapeParam, this.triggerShapeParam) + '\r\n';
    if (this.xdTransformerTypeID !== null) xml += xmlGetElement(tagGuids.tagXDTransformerTypeID, this.xdTransformerTypeID) + '\r\n';
    if (this.xdSystemSideRatio !== null) xml += xmlGetElement(tagGuids.tagXDSystemSideRatio, this.xdSystemSideRatio) + '\r\n';
    if (this.xdMonitorSideRatio !== null) xml += xmlGetElement(tagGuids.tagXDMonitorSideRatio, this.xdMonitorSideRatio) + '\r\n';
    if (this.xdFrequencyResponse !== null) xml += xmlGetElement(tagGuids.tagXDFrequencyResponse, this.xdFrequencyResponse) + '\r\n';
    if (this.calTimeSkew !== null) xml += xmlGetElement(tagGuids.tagCalTimeSkew, this.calTimeSkew) + '\r\n';
    if (this.calOffset !== null) xml += xmlGetElement(tagGuids.tagCalOffset, this.calOffset) + '\r\n';
    if (this.calRatio !== null) xml += xmlGetElement(tagGuids.tagCalRatio, this.calRatio) + '\r\n';
    if (this.calMustUseARCal !== null) xml += xmlGetElement(tagGuids.tagCalMustUseARCal, this.calMustUseARCal) + '\r\n';
    if (this.calApplied !== null) xml += xmlGetElement(tagGuids.tagCalApplied, this.calApplied) + '\r\n';
    if (this.calRecorded !== null) xml += xmlGetElement(tagGuids.tagCalRecorded, this.calRecorded) + '\r\n';
    if (this.triggerHighHigh !== null) xml += xmlGetElement(tagGuids.tagTriggerHighHigh, this.triggerHighHigh) + '\r\n';
    if (this.triggerHigh !== null) xml += xmlGetElement(tagGuids.tagTriggerHigh, this.triggerHigh) + '\r\n';
    if (this.triggerLow !== null) xml += xmlGetElement(tagGuids.tagTriggerLow, this.triggerLow) + '\r\n';
    if (this.triggerLowLow !== null) xml += xmlGetElement(tagGuids.tagTriggerLowLow, this.triggerLowLow) + '\r\n';
    if (this.triggerDeadBand !== null) xml += xmlGetElement(tagGuids.tagTriggerDeadBand, this.triggerDeadBand) + '\r\n';
    if (this.triggerRate !== null) xml += xmlGetElement(tagGuids.tagTriggerRate, this.triggerRate) + '\r\n';

    xml += '</tagOneChannelSetting>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      channelDefnIdx: this.channelDefnIdx,
      triggerTypeID: this.triggerTypeID,
      fullScale: this.fullScale,
      noiseFloor: this.noiseFloor,
      triggerShapeParam: this.triggerShapeParam,
      transformer: {
        typeID: this.xdTransformerTypeID,
        systemSideRatio: this.xdSystemSideRatio,
        monitorSideRatio: this.xdMonitorSideRatio,
        frequencyResponse: this.xdFrequencyResponse ? Array.from(this.xdFrequencyResponse) : null
      },
      calibration: {
        timeSkew: this.calTimeSkew,
        offset: this.calOffset,
        ratio: this.calRatio,
        mustUseARCal: this.calMustUseARCal,
        applied: this.calApplied ? Array.from(this.calApplied) : null,
        recorded: this.calRecorded ? Array.from(this.calRecorded) : null
      },
      triggerThresholds: {
        highHigh: this.triggerHighHigh,
        high: this.triggerHigh,
        low: this.triggerLow,
        lowLow: this.triggerLowLow,
        deadBand: this.triggerDeadBand,
        rate: this.triggerRate
      }
    };
  }
}

// Helpers
function _getUintFromElement(el) {
  if (el && typeof el.getValueUINT4 === 'function') {
    const r = el.getValueUINT4();
    if (r && r.status) return r.value;
  }
  if (el && typeof el.getValueAsDouble === 'function') {
    const r = el.getValueAsDouble(0);
    if (r && r.status) return r.value;
  }
  return 0;
}

function _getDoubleFromElement(el) {
  if (el && typeof el.getValueREAL8 === 'function') {
    const r = el.getValueREAL8();
    if (r && r.status) return r.value;
  }
  if (el && typeof el.getValueAsDouble === 'function') {
    const r = el.getValueAsDouble(0);
    if (r && r.status) return r.value;
  }
  return 0;
}

function _getDoubleArrayFromElement(el) {
  if (el && typeof el.getValuesArray === 'function') {
    return el.getValuesArray(el.getCount ? el.getCount() : 0);
  }
  return null;
}

module.exports = ChannelSetting;
