'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement } = require('./utilities');

/**
 * Custom source info collection - ported from CustomSourceInfo.cs.
 * Contains instrument type, model, contact info, etc.
 */
class CustomSourceInfo {
  constructor() {
    /** @type {Buffer} Instrument type GUID */
    this.instrumentTypeID = null;

    /** @type {string} Instrument model name */
    this.instrumentModelName = '';

    /** @type {string} Instrument model number */
    this.instrumentModelNumber = '';

    /** @type {string} Contact name */
    this.name = '';

    /** @type {string} Address line 1 */
    this.address1 = '';

    /** @type {string} Address line 2 */
    this.address2 = '';

    /** @type {string} City */
    this.city = '';

    /** @type {string} State or province */
    this.state = '';

    /** @type {string} Postal code */
    this.postalCode = '';

    /** @type {string} Country */
    this.country = '';

    /** @type {string} Phone number */
    this.phoneVoice = '';

    /** @type {string} Fax number */
    this.phoneFAX = '';

    /** @type {string} Email address */
    this.email = '';

    /** @type {Logger} Application logger */
    this.loggerApplication = new Logger();

    /** @type {Logger} Compliance logger */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll - The collection element
   */
  static fromCollection(coll) {
    const info = new CustomSourceInfo();
    if (!coll) return info;

    const getCount = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < getCount; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;

      const tag = el.getTag();
      try {
        if (guidEquals(tag, tagGuids.tagInstrumentTypeID)) {
          const r = el.getValueGUID ? el.getValueGUID() : null;
          if (r && r.status) info.instrumentTypeID = r.value;
        } else if (guidEquals(tag, tagGuids.tagInstrumentModelName)) {
          info.instrumentModelName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagInstrumentModelNumber)) {
          info.instrumentModelNumber = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagName)) {
          info.name = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagAddress1)) {
          info.address1 = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagAddress2)) {
          info.address2 = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCity)) {
          info.city = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagState)) {
          info.state = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPostalCode)) {
          info.postalCode = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCountry)) {
          info.country = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhoneVoice)) {
          info.phoneVoice = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhoneFAX)) {
          info.phoneFAX = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagEMail)) {
          info.email = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // blank tag - ignore
        } else {
          info.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        info.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    return info;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagCustomSourceInfo>\r\n';
    if (applicationLog) {
      xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    }
    if (complianceLog) {
      xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';
    }
    if (this.instrumentTypeID) xml += xmlGetElement(tagGuids.tagInstrumentTypeID, this.instrumentTypeID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagInstrumentModelName, this.instrumentModelName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagInstrumentModelNumber, this.instrumentModelNumber) + '\r\n';
    xml += xmlGetElement(tagGuids.tagName, this.name) + '\r\n';
    xml += xmlGetElement(tagGuids.tagAddress1, this.address1) + '\r\n';
    xml += xmlGetElement(tagGuids.tagAddress2, this.address2) + '\r\n';
    xml += xmlGetElement(tagGuids.tagCity, this.city) + '\r\n';
    xml += xmlGetElement(tagGuids.tagState, this.state) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPostalCode, this.postalCode) + '\r\n';
    xml += xmlGetElement(tagGuids.tagCountry, this.country) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPhoneVoice, this.phoneVoice) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPhoneFAX, this.phoneFAX) + '\r\n';
    xml += xmlGetElement(tagGuids.tagEMail, this.email) + '\r\n';
    xml += '</tagCustomSourceInfo>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      instrumentTypeID: this.instrumentTypeID ? this.instrumentTypeID.toString('hex') : null,
      instrumentModelName: this.instrumentModelName || '',
      instrumentModelNumber: this.instrumentModelNumber || '',
      contact: {
        name: this.name || '',
        address1: this.address1 || '',
        address2: this.address2 || '',
        city: this.city || '',
        state: this.state || '',
        postalCode: this.postalCode || '',
        country: this.country || '',
        phoneVoice: this.phoneVoice || '',
        phoneFAX: this.phoneFAX || '',
        email: this.email || ''
      }
    };
  }
}

/**
 * Extract string from a Vector element.
 * @param {import('../core/vector').Vector} el
 * @returns {string}
 */
function _getStringFromElement(el) {
  if (el && typeof el.getValues === 'function') {
    const out = { value: '' };
    el.getValues(out);
    return out.value;
  }
  if (el && typeof el.getString === 'function') {
    return el.getString();
  }
  return '';
}

module.exports = CustomSourceInfo;
