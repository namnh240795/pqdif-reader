'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement, pqdifTimestampToDate } = require('./utilities');
const ChannelSetting = require('./channelSetting');

/**
 * Monitor settings record wrapper - ported from RecMonitorSettings.cs.
 */
class MonitorSettings {
  constructor() {
    /** @type {Date} Effective time */
    this.effective = new Date(0);

    /** @type {Date} Time installed */
    this.timeInstalled = new Date(0);

    /** @type {Date|null} Time removed */
    this.timeRemoved = null;

    /** @type {boolean} Use calibration */
    this.useCalibration = false;

    /** @type {boolean} Use transducer */
    this.useTransducer = false;

    /** @type {ChannelSetting[]} Channel settings */
    this.channelSettingsArray = [];

    /** @type {number|null} Nominal frequency in Hz */
    this.nominalFrequency = null;

    /** @type {number|null} Physical connection */
    this.settingPhysicalConnection = null;

    /** @type {string} Comments */
    this.comments = '';

    /** @type {number|null} LST to local offset in seconds */
    this.lsttoLocal = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {MonitorSettings}
   */
  static fromCollection(coll) {
    const ms = new MonitorSettings();
    if (!coll) return ms;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagEffective)) {
          ms.effective = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimeInstalled)) {
          ms.timeInstalled = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimeRemoved)) {
          ms.timeRemoved = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagUseCalibration)) {
          ms.useCalibration = _getBoolFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagUseTransducer)) {
          ms.useTransducer = _getBoolFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChannelSettingsArray)) {
          // Channel settings sub-collection
          if (typeof el.getCount === 'function') {
            const csCount = el.getCount();
            if (csCount === 0) {
              ms.loggerCompliance.log('tagChannelSettingsArray Must have at least tagOneChannelSetting Record.', LogLevels.Error);
            }
            for (let j = 0; j < csCount; j++) {
              const csEl = el.getElement(j);
              if (csEl && typeof csEl.getCount === 'function') {
                const cs = ChannelSetting.fromCollection(csEl);
                ms.channelSettingsArray.push(cs);
              }
            }
          }
        } else if (guidEquals(tag, tagGuids.tagNominalFrequency)) {
          ms.nominalFrequency = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSettingPhysicalConnection)) {
          ms.settingPhysicalConnection = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagComments)) {
          ms.comments = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLSTtoLocal)) {
          ms.lsttoLocal = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          ms.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        ms.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    // Check required tags
    if (ms.channelSettingsArray.length === 0) {
      ms.loggerCompliance.log('Required tagChannelSettingsArray is Missing.', LogLevels.Error);
    }

    return ms;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagRecMonitorSettings>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagEffective, this.effective) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTimeInstalled, this.timeInstalled) + '\r\n';
    if (this.timeRemoved !== null) xml += xmlGetElement(tagGuids.tagTimeRemoved, this.timeRemoved) + '\r\n';
    xml += xmlGetElement(tagGuids.tagUseCalibration, this.useCalibration) + '\r\n';
    xml += xmlGetElement(tagGuids.tagUseTransducer, this.useTransducer) + '\r\n';

    xml += '<tagChannelSettingsArray>\r\n';
    for (const cs of this.channelSettingsArray) {
      xml += cs.getXML(applicationLog, complianceLog);
    }
    xml += '</tagChannelSettingsArray>\r\n';

    if (this.nominalFrequency !== null) xml += xmlGetElement(tagGuids.tagNominalFrequency, this.nominalFrequency) + '\r\n';
    if (this.settingPhysicalConnection !== null) xml += xmlGetElement(tagGuids.tagSettingPhysicalConnection, this.settingPhysicalConnection) + '\r\n';
    xml += xmlGetElement(tagGuids.tagComments, this.comments) + '\r\n';
    if (this.lsttoLocal !== null) xml += xmlGetElement(tagGuids.tagLSTtoLocal, this.lsttoLocal) + '\r\n';

    xml += '</tagRecMonitorSettings>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      recordType: 'MonitorSettings',
      effective: this.effective ? this.effective.toISOString() : null,
      timeInstalled: this.timeInstalled ? this.timeInstalled.toISOString() : null,
      timeRemoved: this.timeRemoved ? this.timeRemoved.toISOString() : null,
      useCalibration: this.useCalibration,
      useTransducer: this.useTransducer,
      nominalFrequency: this.nominalFrequency,
      physicalConnection: this.settingPhysicalConnection,
      comments: this.comments || '',
      lsttoLocal: this.lsttoLocal,
      channelSettings: this.channelSettingsArray.map(cs => cs.getJSON())
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

function _getBoolFromElement(el) {
  if (el && typeof el.getValueBOOL4 === 'function') {
    const r = el.getValueBOOL4();
    if (r && r.status) return r.value;
  }
  if (el && typeof el.getValueAsDouble === 'function') {
    const r = el.getValueAsDouble(0);
    if (r && r.status) return r.value !== 0;
  }
  return false;
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

module.exports = MonitorSettings;
