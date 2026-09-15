'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement } = require('./utilities');

/**
 * Series definition - ported from SeriesDefinition.cs (OneSeriesDefn).
 */
class SeriesDefinition {
  constructor() {
    /** @type {Buffer} Value type GUID */
    this.valueTypeID = null;

    /** @type {number} Quantity units ID */
    this.quantityUnitsID = 0;

    /** @type {Buffer} Quantity characteristic GUID */
    this.quantityCharacteristicID = null;

    /** @type {number|null} Quantity significant digits */
    this.quantitySignificantDigitsID = null;

    /** @type {number|null} Quantity resolution */
    this.quantityResolutionID = null;

    /** @type {number} Storage method ID */
    this.storageMethodID = 0;

    /** @type {string} Value type name */
    this.valueTypeName = '';

    /** @type {number|null} Hint Greek prefix */
    this.hintGreekPrefixID = null;

    /** @type {number|null} Hint preferred units */
    this.hintPreferredUnitsID = null;

    /** @type {number|null} Hint default display */
    this.hintDefaultDisplayID = null;

    /** @type {number|null} Probability interval */
    this.probInterval = null;

    /** @type {number|null} Probability percentile */
    this.probPercentile = null;

    /** @type {number|null} Series nominal quantity */
    this.seriesNominalQuantity = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {SeriesDefinition}
   */
  static fromCollection(coll) {
    const defn = new SeriesDefinition();
    if (!coll) return defn;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagValueTypeID)) {
          const r = el.getValueGUID ? el.getValueGUID() : null;
          if (r && r.status) defn.valueTypeID = r.value;
        } else if (guidEquals(tag, tagGuids.tagQuantityUnitsID)) {
          defn.quantityUnitsID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagQuantityCharacteristicID)) {
          const r = el.getValueGUID ? el.getValueGUID() : null;
          if (r && r.status) defn.quantityCharacteristicID = r.value;
        } else if (guidEquals(tag, tagGuids.tagQuantitySignificantDigitsID)) {
          defn.quantitySignificantDigitsID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagQuantityResolutionID)) {
          defn.quantityResolutionID = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagStorageMethodID)) {
          defn.storageMethodID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagValueTypeName)) {
          defn.valueTypeName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagHintGreekPrefixID)) {
          defn.hintGreekPrefixID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagHintPreferredUnitsID)) {
          defn.hintPreferredUnitsID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagHintDefaultDisplayID)) {
          defn.hintDefaultDisplayID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagProbInterval)) {
          defn.probInterval = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagProbPercentile)) {
          defn.probPercentile = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesNominalQuantity)) {
          defn.seriesNominalQuantity = _getDoubleFromElement(el);
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
    if (!defn.valueTypeID) defn.loggerCompliance.log('Required tagValueTypeID is Missing.', LogLevels.Error);
    if (count === 0) defn.loggerCompliance.log('A Series Definition Collection Should have at least One Element.', LogLevels.Error);

    return defn;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagOneSeriesDefn>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    if (this.valueTypeID) xml += xmlGetElement(tagGuids.tagValueTypeID, this.valueTypeID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagQuantityUnitsID, this.quantityUnitsID) + '\r\n';
    if (this.quantityCharacteristicID) xml += xmlGetElement(tagGuids.tagQuantityCharacteristicID, this.quantityCharacteristicID) + '\r\n';
    if (this.quantitySignificantDigitsID !== null) xml += xmlGetElement(tagGuids.tagQuantitySignificantDigitsID, this.quantitySignificantDigitsID) + '\r\n';
    if (this.quantityResolutionID !== null) xml += xmlGetElement(tagGuids.tagQuantityResolutionID, this.quantityResolutionID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagStorageMethodID, this.storageMethodID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagValueTypeName, this.valueTypeName) + '\r\n';
    if (this.hintGreekPrefixID !== null) xml += xmlGetElement(tagGuids.tagHintGreekPrefixID, this.hintGreekPrefixID) + '\r\n';
    if (this.hintPreferredUnitsID !== null) xml += xmlGetElement(tagGuids.tagHintPreferredUnitsID, this.hintPreferredUnitsID) + '\r\n';
    if (this.hintDefaultDisplayID !== null) xml += xmlGetElement(tagGuids.tagHintDefaultDisplayID, this.hintDefaultDisplayID) + '\r\n';
    if (this.probInterval !== null) xml += xmlGetElement(tagGuids.tagProbInterval, this.probInterval) + '\r\n';
    if (this.probPercentile !== null) xml += xmlGetElement(tagGuids.tagProbPercentile, this.probPercentile) + '\r\n';
    if (this.seriesNominalQuantity !== null) xml += xmlGetElement(tagGuids.tagProbPercentile, this.seriesNominalQuantity) + '\r\n';

    xml += '</tagOneSeriesDefn>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      valueTypeID: this.valueTypeID ? this.valueTypeID.toString('hex') : null,
      quantityUnitsID: this.quantityUnitsID,
      quantityCharacteristicID: this.quantityCharacteristicID ? this.quantityCharacteristicID.toString('hex') : null,
      quantitySignificantDigitsID: this.quantitySignificantDigitsID,
      quantityResolutionID: this.quantityResolutionID,
      storageMethodID: this.storageMethodID,
      valueTypeName: this.valueTypeName || '',
      hintGreekPrefixID: this.hintGreekPrefixID,
      hintPreferredUnitsID: this.hintPreferredUnitsID,
      hintDefaultDisplayID: this.hintDefaultDisplayID,
      probInterval: this.probInterval,
      probPercentile: this.probPercentile,
      seriesNominalQuantity: this.seriesNominalQuantity
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

module.exports = SeriesDefinition;
