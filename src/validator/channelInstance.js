'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement } = require('./utilities');
const SeriesInstance = require('./seriesInstance');

/**
 * Channel instance - ported from OneChannelInst.cs.
 */
class ChannelInstance {
  constructor() {
    /** @type {number} Channel definition index */
    this.channelDefnIdx = 0;

    /** @type {SeriesInstance[]} Series instances */
    this.seriesInstances = [];

    /** @type {number|null} Characterization magnitude */
    this.charactMagnitude = null;

    /** @type {number|null} Characterization frequency */
    this.charactFrequency = null;

    /** @type {number|null} Channel trigger module info */
    this.chanTriggerModuleInfo = null;

    /** @type {string} Channel trigger module name */
    this.chanTriggerModuleName = '';

    /** @type {string} Cross trigger device name */
    this.crossTriggerDeviceName = '';

    /** @type {number|null} Cross trigger channel index */
    this.crossTriggerChanIdx = null;

    /** @type {number|null} Channel trigger type ID */
    this.chanTriggerTypeID = null;

    /** @type {number|null} Channel frequency (Hz) */
    this.channelFrequency = null;

    /** @type {number|null} Channel group ID */
    this.channelGroupID = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {ChannelInstance}
   */
  static fromCollection(coll) {
    const ci = new ChannelInstance();
    if (!coll) return ci;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagChannelDefnIdx)) {
          ci.channelDefnIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactMagnitude)) {
          ci.charactMagnitude = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCharactFrequency)) {
          ci.charactFrequency = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChanTriggerModuleInfo)) {
          ci.chanTriggerModuleInfo = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChanTriggerModuleName)) {
          ci.chanTriggerModuleName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCrossTriggerDeviceName)) {
          ci.crossTriggerDeviceName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCrossTriggerChanIdx)) {
          ci.crossTriggerChanIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChanTriggerTypeID)) {
          ci.chanTriggerTypeID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChannelFrequency)) {
          ci.channelFrequency = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChannelGroupID)) {
          ci.channelGroupID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesInstances)) {
          // Series instances sub-collection
          if (typeof el.getCount === 'function') {
            const siCount = el.getCount();
            if (siCount === 0) {
              ci.loggerCompliance.log('tagSeriesInstances must have at least one tagOneSeriesInstance record.', LogLevels.Error);
            }
            for (let j = 0; j < siCount; j++) {
              const siEl = el.getElement(j);
              if (siEl && typeof siEl.getCount === 'function') {
                const si = SeriesInstance.fromCollection(siEl);
                ci.seriesInstances.push(si);
              }
            }
          }
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          ci.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        ci.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    // Check required tags
    if (ci.channelDefnIdx === 0) ci.loggerCompliance.log('Required tagChannelDefnIdx is Missing.', LogLevels.Error);
    if (ci.seriesInstances.length === 0) ci.loggerCompliance.log('Required tagSeriesInstances is Missing.', LogLevels.Error);

    return ci;
  }

  /**
   * Get the channel definition that corresponds to this channel instance.
   * @param {import('./dataSource').DataSource} dataSource
   * @returns {import('./channelDefinition').ChannelDefinition|null}
   */
  getChannelDefn(dataSource) {
    if (!dataSource) return null;
    const idx = this.channelDefnIdx;
    if (idx < dataSource.channelDefns.length) {
      return dataSource.channelDefns[idx];
    }
    return null;
  }

  /**
   * Get the series instance to use, resolving shared series references.
   * @param {import('./observation').Observation} observation
   * @param {SeriesInstance} seriesInstance
   * @returns {SeriesInstance}
   */
  getSeriesInstance(observation, seriesInstance) {
    if (seriesInstance.seriesValues === null &&
        seriesInstance.seriesShareChannelIdx !== null &&
        seriesInstance.seriesShareSeriesIdx !== null) {
      const channelIdx = seriesInstance.seriesShareChannelIdx;
      const seriesIdx = seriesInstance.seriesShareSeriesIdx;
      if (channelIdx < observation.channelInstances.length) {
        const sharedChannel = observation.channelInstances[channelIdx];
        if (seriesIdx < sharedChannel.seriesInstances.length) {
          return sharedChannel.seriesInstances[seriesIdx];
        }
      }
    }
    return seriesInstance;
  }

  /**
   * Get min/max timestamps from the time series in this channel instance.
   * @param {import('./observation').Observation} observation
   * @param {import('./channelDefinition').ChannelDefinition} channelDefn
   * @returns {{ min: Date|null, max: Date|null }}
   */
  getTimeStartMinMax(observation, channelDefn) {
    const idValueTypeTime = require('../constants/idGuids').ID_SERIES_VALUE_TYPE_TIME;

    // Find the time series definition index
    let seriesDefnIdxTime = -1;
    let storageMethodIdTime = 0;
    for (let i = 0; i < channelDefn.seriesDefns.length; i++) {
      if (guidEquals(channelDefn.seriesDefns[i].valueTypeID, idValueTypeTime)) {
        seriesDefnIdxTime = i;
        storageMethodIdTime = channelDefn.seriesDefns[i].storageMethodID;
        break;
      }
    }

    if (seriesDefnIdxTime >= 0 && seriesDefnIdxTime < this.seriesInstances.length) {
      const timeSeries = this.seriesInstances[seriesDefnIdxTime];
      if (timeSeries.seriesValues && timeSeries.seriesValues.length > 0) {
        const resolved = this.getSeriesInstance(observation, timeSeries);
        const startTimes = resolved.getSeriesValues(storageMethodIdTime);
        if (startTimes && startTimes.length > 0) {
          const startMs = observation.timeStart.getTime();
          let tsMin = startMs + startTimes[0] * 1000;
          let tsMax = tsMin;
          const lastTime = startMs + startTimes[startTimes.length - 1] * 1000;
          if (lastTime < tsMin) tsMin = lastTime;
          if (lastTime > tsMax) tsMax = lastTime;
          return { min: new Date(tsMin), max: new Date(tsMax) };
        }
      }
    }

    return { min: null, max: null };
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @param {number|null} maxSeriesValues
   * @returns {string}
   */
  getXML(applicationLog, complianceLog, maxSeriesValues) {
    let xml = '<tagOneChannelInst>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagChannelDefnIdx, this.channelDefnIdx) + '\r\n';

    xml += '<tagSeriesInstances>\r\n';
    for (const si of this.seriesInstances) {
      xml += si.getXML(applicationLog, complianceLog, maxSeriesValues);
    }
    xml += '</tagSeriesInstances>\r\n';

    if (this.charactMagnitude !== null) xml += xmlGetElement(tagGuids.tagCharactMagnitude, this.charactMagnitude) + '\r\n';
    if (this.charactFrequency !== null) xml += xmlGetElement(tagGuids.tagCharactFrequency, this.charactFrequency) + '\r\n';
    if (this.chanTriggerModuleInfo !== null) xml += xmlGetElement(tagGuids.tagChanTriggerModuleInfo, this.chanTriggerModuleInfo) + '\r\n';
    xml += xmlGetElement(tagGuids.tagChanTriggerModuleName, this.chanTriggerModuleName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagCrossTriggerDeviceName, this.crossTriggerDeviceName) + '\r\n';
    if (this.crossTriggerChanIdx !== null) xml += xmlGetElement(tagGuids.tagCrossTriggerChanIdx, this.crossTriggerChanIdx) + '\r\n';
    if (this.chanTriggerTypeID !== null) xml += xmlGetElement(tagGuids.tagChanTriggerTypeID, this.chanTriggerTypeID) + '\r\n';
    if (this.channelFrequency !== null) xml += xmlGetElement(tagGuids.tagChannelFrequency, this.channelFrequency) + '\r\n';
    if (this.channelGroupID !== null) xml += xmlGetElement(tagGuids.tagChannelGroupID, this.channelGroupID) + '\r\n';

    xml += '</tagOneChannelInst>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @param {number|null} maxSeriesValues
   * @returns {Object}
   */
  getJSON(maxSeriesValues) {
    return {
      channelDefnIdx: this.channelDefnIdx,
      seriesInstances: this.seriesInstances.map(si => si.getJSON(maxSeriesValues)),
      characterization: {
        magnitude: this.charactMagnitude,
        frequency: this.charactFrequency
      },
      triggerModuleInfo: this.chanTriggerModuleInfo,
      triggerModuleName: this.chanTriggerModuleName || '',
      crossTriggerDeviceName: this.crossTriggerDeviceName || '',
      crossTriggerChanIdx: this.crossTriggerChanIdx,
      triggerTypeID: this.chanTriggerTypeID,
      channelFrequency: this.channelFrequency,
      channelGroupID: this.channelGroupID
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

function _getStringFromElement(el) {
  if (el && typeof el.getValues === 'function') {
    const out = { value: '' };
    el.getValues(out);
    return out.value;
  }
  if (el && typeof el.getString === 'function') return el.getString();
  return '';
}

module.exports = ChannelInstance;
