'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement, pqdifTimestampToDate } = require('./utilities');
const ChannelInstance = require('./channelInstance');

/**
 * Observation record wrapper - ported from RecObservation.cs.
 */
class Observation {
  constructor() {
    /** @type {string} Observation name */
    this.observationName = '';

    /** @type {Date} Time created */
    this.timeCreate = new Date(0);

    /** @type {Date} Time start */
    this.timeStart = new Date(0);

    /** @type {number} Trigger method ID */
    this.triggerMethodID = 0;

    /** @type {Date|null} Time triggered */
    this.timeTriggered = null;

    /** @type {number[]|null} Channel trigger indices */
    this.channelTriggerIdx = null;

    /** @type {number|null} Observation serial */
    this.observationSerial = null;

    /** @type {number|null} Observation aggregation serial */
    this.observationAggregationSerial = null;

    /** @type {Buffer} Disturbance category GUID */
    this.disturbanceCategoryID = null;

    /** @type {ChannelInstance[]} Channel instances */
    this.channelInstances = [];

    /** @type {string} Comments */
    this.comments = '';

    /** @type {number|null} Characterization disturbance direction */
    this.charactDisturbDirection = null;

    /** @type {number|null} Characterization disturbance direction quality */
    this.charactDisturbDirectionQuality = null;

    /** @type {number|null} Min voltage (per unit) */
    this.charactMinVoltagePU = null;

    /** @type {number|null} Max voltage (per unit) */
    this.charactMaxVoltagePU = null;

    /** @type {number|null} Min voltage (volts) */
    this.charactMinVoltage = null;

    /** @type {number|null} Max voltage (volts) */
    this.charactMaxVoltage = null;

    /** @type {number|null} Min current (amps) */
    this.charactMinCurrent = null;

    /** @type {number|null} Max current (amps) */
    this.charactMaxCurrent = null;

    /** @type {number|null} Characterization duration */
    this.charactDuration = null;

    /** @type {Buffer} Sag type GUID */
    this.sagTypeID = null;

    /** @type {number|null} LST to local offset */
    this.lsttoLocal = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {Observation}
   */
  static fromCollection(coll) {
    const obs = new Observation();
    if (!coll) return obs;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagObservationName)) {
          obs.observationName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimeCreate)) {
          obs.timeCreate = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimeStart)) {
          obs.timeStart = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTriggerMethodID)) {
          obs.triggerMethodID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimeTriggered)) {
          obs.timeTriggered = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChannelTriggerIdx)) {
          obs.channelTriggerIdx = _getUintArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagObservationSerial)) {
          obs.observationSerial = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagObservationAggregationSerial)) {
          obs.observationAggregationSerial = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagDisturbanceCategoryID)) {
          obs.disturbanceCategoryID = _getGuidFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagComments)) {
          obs.comments = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMinVoltagePU)) {
          obs.charactMinVoltagePU = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMaxVoltagePU)) {
          obs.charactMaxVoltagePU = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMinVoltage)) {
          obs.charactMinVoltage = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMaxVoltage)) {
          obs.charactMaxVoltage = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMinCurrent)) {
          obs.charactMinCurrent = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMaxCurrent)) {
          obs.charactMaxCurrent = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSagTypeID)) {
          obs.sagTypeID = _getGuidFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLSTtoLocal)) {
          obs.lsttoLocal = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChannelInstances)) {
          // Channel instances sub-collection
          if (typeof el.getCount === 'function') {
            const ciCount = el.getCount();
            if (ciCount === 0) {
              obs.loggerCompliance.log('tagChannelInstances must have at least one tagOneChannelInst Record.', LogLevels.Error);
            }
            for (let j = 0; j < ciCount; j++) {
              const ciEl = el.getElement(j);
              if (ciEl && typeof ciEl.getCount === 'function') {
                const ci = ChannelInstance.fromCollection(ciEl);
                obs.channelInstances.push(ci);
              }
            }
          }
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          obs.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        obs.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    return obs;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @param {number|null} maxSeriesValues
   * @returns {string}
   */
  getXML(applicationLog, complianceLog, maxSeriesValues) {
    let xml = '<tagRecObservation>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagObservationName, this.observationName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTimeCreate, this.timeCreate) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTimeStart, this.timeStart) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTriggerMethodID, this.triggerMethodID) + '\r\n';
    if (this.timeTriggered !== null) xml += xmlGetElement(tagGuids.tagTimeTriggered, this.timeTriggered) + '\r\n';
    if (this.channelTriggerIdx !== null) xml += xmlGetElement(tagGuids.tagChannelTriggerIdx, this.channelTriggerIdx) + '\r\n';
    if (this.observationSerial !== null) xml += xmlGetElement(tagGuids.tagObservationSerial, this.observationSerial) + '\r\n';
    if (this.observationAggregationSerial !== null) xml += xmlGetElement(tagGuids.tagObservationAggregationSerial, this.observationAggregationSerial) + '\r\n';

    xml += '<tagChannelInstances>\r\n';
    for (const ci of this.channelInstances) {
      xml += ci.getXML(applicationLog, complianceLog, maxSeriesValues);
    }
    xml += '</tagChannelInstances>\r\n';

    xml += xmlGetElement(tagGuids.tagComments, this.comments) + '\r\n';
    if (this.charactDisturbDirection !== null) xml += xmlGetElement(tagGuids.tagCharactDisturbDirection, this.charactDisturbDirection) + '\r\n';
    if (this.charactDisturbDirectionQuality !== null) xml += xmlGetElement(tagGuids.tagCharactDisturbDirectionQuality, this.charactDisturbDirectionQuality) + '\r\n';
    if (this.charactDuration !== null) xml += xmlGetElement(tagGuids.tagCharactDuration, this.charactDuration) + '\r\n';
    if (this.charactMinVoltagePU !== null) xml += xmlGetElement(tagGuids.tagCharactMinVoltagePU, this.charactMinVoltagePU) + '\r\n';
    if (this.charactMaxVoltagePU !== null) xml += xmlGetElement(tagGuids.tagCharactMaxVoltagePU, this.charactMaxVoltagePU) + '\r\n';
    if (this.charactMinVoltage !== null) xml += xmlGetElement(tagGuids.tagCharactMinVoltage, this.charactMinVoltage) + '\r\n';
    if (this.charactMaxVoltage !== null) xml += xmlGetElement(tagGuids.tagCharactMaxVoltage, this.charactMaxVoltage) + '\r\n';
    if (this.charactMinCurrent !== null) xml += xmlGetElement(tagGuids.tagCharactMinCurrent, this.charactMinCurrent) + '\r\n';
    if (this.charactMaxCurrent !== null) xml += xmlGetElement(tagGuids.tagCharactMaxCurrent, this.charactMaxCurrent) + '\r\n';
    if (this.sagTypeID) xml += xmlGetElement(tagGuids.tagSagTypeID, this.sagTypeID) + '\r\n';
    if (this.lsttoLocal !== null) xml += xmlGetElement(tagGuids.tagLSTtoLocal, this.lsttoLocal) + '\r\n';

    xml += '</tagRecObservation>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @param {number|null} maxSeriesValues
   * @returns {Object}
   */
  getJSON(maxSeriesValues) {
    return {
      recordType: 'Observation',
      observationName: this.observationName || '',
      timeCreate: this.timeCreate ? this.timeCreate.toISOString() : null,
      timeStart: this.timeStart ? this.timeStart.toISOString() : null,
      triggerMethodID: this.triggerMethodID,
      timeTriggered: this.timeTriggered ? this.timeTriggered.toISOString() : null,
      channelTriggerIdx: this.channelTriggerIdx ? Array.from(this.channelTriggerIdx) : null,
      observationSerial: this.observationSerial,
      observationAggregationSerial: this.observationAggregationSerial,
      disturbanceCategoryID: this.disturbanceCategoryID ? this.disturbanceCategoryID.toString('hex') : null,
      comments: this.comments || '',
      sagTypeID: this.sagTypeID ? this.sagTypeID.toString('hex') : null,
      lsttoLocal: this.lsttoLocal,
      characterization: {
        disturbDirection: this.charactDisturbDirection,
        disturbDirectionQuality: this.charactDisturbDirectionQuality,
        minVoltagePU: this.charactMinVoltagePU,
        maxVoltagePU: this.charactMaxVoltagePU,
        minVoltage: this.charactMinVoltage,
        maxVoltage: this.charactMaxVoltage,
        minCurrent: this.charactMinCurrent,
        maxCurrent: this.charactMaxCurrent,
        duration: this.charactDuration
      },
      channelInstances: this.channelInstances.map(ci => ci.getJSON(maxSeriesValues))
    };
  }
}

// Helpers
function _getGuidFromElement(el) {
  if (el && typeof el.getValueGUID === 'function') {
    const r = el.getValueGUID();
    if (r && r.status) return r.value;
  }
  return null;
}

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

function _getStringFromElement(el) {
  if (el && typeof el.getValues === 'function') {
    const out = { value: '' };
    el.getValues(out);
    return out.value;
  }
  if (el && typeof el.getString === 'function') return el.getString();
  return '';
}

function _getDateFromElement(el) {
  if (el && typeof el.getValueTimeStamp === 'function') {
    const r = el.getValueTimeStamp();
    if (r && r.status) return pqdifTimestampToDate(r.value);
  }
  return new Date(0);
}

function _getUintArrayFromElement(el) {
  if (el && typeof el.getValuesArray === 'function') {
    return el.getValuesArray(el.getCount ? el.getCount() : 0);
  }
  return null;
}

module.exports = Observation;
