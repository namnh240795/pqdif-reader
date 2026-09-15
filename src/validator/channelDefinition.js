'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement } = require('./utilities');
const SeriesDefinition = require('./seriesDefinition');

/**
 * Channel definition - ported from OneChannelDefn.cs.
 */
class ChannelDefinition {
  constructor() {
    /** @type {string} Channel name */
    this.channelName = '';

    /** @type {number} Phase ID */
    this.phaseID = 0;

    /** @type {string} Other channel identifier */
    this.otherChannelIdentifier = '';

    /** @type {string} Group name */
    this.groupName = '';

    /** @type {Buffer} Quantity type GUID */
    this.quantityTypeID = null;

    /** @type {number} Quantity measured ID */
    this.quantityMeasuredID = 0;

    /** @type {number} Physical channel */
    this.physicalChannel = 0;

    /** @type {string} Quantity name */
    this.quantityName = '';

    /** @type {number} Primary series index */
    this.primarySeriesIdx = 0;

    /** @type {SeriesDefinition[]} Series definitions */
    this.seriesDefns = [];

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {ChannelDefinition}
   */
  static fromCollection(coll) {
    const defn = new ChannelDefinition();
    if (!coll) return defn;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagChannelName)) {
          defn.channelName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhaseID)) {
          defn.phaseID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagOtherChannelIdentifier)) {
          defn.otherChannelIdentifier = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagGroupName)) {
          defn.groupName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagQuantityTypeID)) {
          const r = el.getValueGUID ? el.getValueGUID() : null;
          if (r && r.status) defn.quantityTypeID = r.value;
        } else if (guidEquals(tag, tagGuids.tagQuantityMeasuredID)) {
          defn.quantityMeasuredID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhysicalChannel)) {
          defn.physicalChannel = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagQuantityName)) {
          defn.quantityName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPrimarySeriesIdx)) {
          defn.primarySeriesIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesDefns)) {
          // Series definitions sub-collection
          if (typeof el.getCount === 'function') {
            const seriesCount = el.getCount();
            if (seriesCount === 0) {
              defn.loggerCompliance.log('tagSeriesDefns must have at least one tagOneSeriesDefn record.', LogLevels.Error);
            }
            for (let j = 0; j < seriesCount; j++) {
              const seriesEl = el.getElement(j);
              if (seriesEl && typeof seriesEl.getCount === 'function') {
                const sd = SeriesDefinition.fromCollection(seriesEl);
                defn.seriesDefns.push(sd);
              }
            }
          }
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          defn.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        defn.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    // Check required tags
    if (count === 0) defn.loggerCompliance.log('tagChannelDefns Must have at least tagOneChannelDefn Record.', LogLevels.Error);

    return defn;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagOneChannelDefn>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagChannelName, this.channelName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPhaseID, this.phaseID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagOtherChannelIdentifier, this.otherChannelIdentifier) + '\r\n';
    xml += xmlGetElement(tagGuids.tagGroupName, this.groupName) + '\r\n';
    if (this.quantityTypeID) xml += xmlGetElement(tagGuids.tagQuantityTypeID, this.quantityTypeID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagQuantityMeasuredID, this.quantityMeasuredID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPhysicalChannel, this.physicalChannel) + '\r\n';
    xml += xmlGetElement(tagGuids.tagQuantityName, this.quantityName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPrimarySeriesIdx, this.primarySeriesIdx) + '\r\n';

    xml += '<tagSeriesDefns>\r\n';
    for (const sd of this.seriesDefns) {
      xml += sd.getXML(applicationLog, complianceLog);
    }
    xml += '</tagSeriesDefns>\r\n';

    xml += '</tagOneChannelDefn>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      channelName: this.channelName || '',
      phaseID: this.phaseID,
      otherChannelIdentifier: this.otherChannelIdentifier || '',
      groupName: this.groupName || '',
      quantityTypeID: this.quantityTypeID ? this.quantityTypeID.toString('hex') : null,
      quantityMeasuredID: this.quantityMeasuredID,
      physicalChannel: this.physicalChannel,
      quantityName: this.quantityName || '',
      primarySeriesIdx: this.primarySeriesIdx,
      seriesDefinitions: this.seriesDefns.map(sd => sd.getJSON())
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

function _getStringFromElement(el) {
  if (el && typeof el.getValues === 'function') {
    const out = { value: '' };
    el.getValues(out);
    return out.value;
  }
  if (el && typeof el.getString === 'function') return el.getString();
  return '';
}

module.exports = ChannelDefinition;
