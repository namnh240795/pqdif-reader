'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement, pqdifTimestampToDate } = require('./utilities');

/**
 * Container record wrapper - ported from Container.cs.
 * Always the first record in a PQDIF file.
 */
class Container {
  constructor() {
    /** @type {number[]} Version info [major, minor, compatMajor, compatMinor] */
    this.versionInfo = [0, 0, 0, 0];

    /** @type {string} Original filename */
    this.fileName = '';

    /** @type {Date} Creation time */
    this.creation = new Date(0);

    /** @type {Date} Last saved time */
    this.lastSaved = new Date(0);

    /** @type {number|null} Times saved */
    this.timesSaved = null;

    /** @type {string} Language */
    this.language = '';

    /** @type {string} Title */
    this.title = '';

    /** @type {string} Subject */
    this.subject = '';

    /** @type {string} Author */
    this.author = '';

    /** @type {string} Keywords */
    this.keywords = '';

    /** @type {string} Comments */
    this.comments = '';

    /** @type {string} Last saved by */
    this.lastSavedBy = '';

    /** @type {string} Application */
    this.application = '';

    /** @type {string} Security */
    this.security = '';

    /** @type {string} Owner */
    this.owner = '';

    /** @type {string} Copyright */
    this.copyright = '';

    /** @type {string} Trademarks */
    this.trademarks = '';

    /** @type {string} Notes */
    this.notes = '';

    /** @type {number|null} Compression style ID */
    this.compressionStyleID = null;

    /** @type {number|null} Compression algorithm ID */
    this.compressionAlgorithmID = null;

    /** @type {string} Contact name */
    this.name = '';

    /** @type {string} Address line 1 */
    this.address1 = '';

    /** @type {string} Address line 2 */
    this.address2 = '';

    /** @type {string} City */
    this.city = '';

    /** @type {string} State */
    this.state = '';

    /** @type {string} Postal code */
    this.postalCode = '';

    /** @type {string} Country */
    this.country = '';

    /** @type {string} Phone */
    this.phoneVoice = '';

    /** @type {string} Fax */
    this.phoneFAX = '';

    /** @type {string} Email */
    this.email = '';

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {Container}
   */
  static fromCollection(coll) {
    const container = new Container();
    if (!coll) return container;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagVersionInfo)) {
          container.versionInfo = _getUintArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagFileName)) {
          container.fileName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCreation)) {
          container.creation = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLastSaved)) {
          container.lastSaved = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimesSaved)) {
          container.timesSaved = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLanguage)) {
          container.language = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTitle)) {
          container.title = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSubject)) {
          container.subject = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagAuthor)) {
          container.author = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagKeywords)) {
          container.keywords = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagComments)) {
          container.comments = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLastSavedBy)) {
          container.lastSavedBy = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagApplication)) {
          container.application = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSecurity)) {
          container.security = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagOwner)) {
          container.owner = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCopyright)) {
          container.copyright = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTrademarks)) {
          container.trademarks = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagNotes)) {
          container.notes = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCompressionStyleID)) {
          container.compressionStyleID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCompressionAlgorithmID)) {
          container.compressionAlgorithmID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagName)) {
          container.name = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagAddress1)) {
          container.address1 = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagAddress2)) {
          container.address2 = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCity)) {
          container.city = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagState)) {
          container.state = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPostalCode)) {
          container.postalCode = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagCountry)) {
          container.country = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhoneVoice)) {
          container.phoneVoice = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhoneFAX)) {
          container.phoneFAX = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagEMail)) {
          container.email = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          container.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        container.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    return container;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagContainer>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagVersionInfo, this.versionInfo) + '\r\n';
    xml += xmlGetElement(tagGuids.tagFileName, this.fileName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagCreation, this.creation) + '\r\n';
    xml += xmlGetElement(tagGuids.tagLastSaved, this.lastSaved) + '\r\n';
    if (this.timesSaved !== null) xml += xmlGetElement(tagGuids.tagTimesSaved, this.timesSaved) + '\r\n';
    xml += xmlGetElement(tagGuids.tagLanguage, this.language) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTitle, this.title) + '\r\n';
    xml += xmlGetElement(tagGuids.tagSubject, this.subject) + '\r\n';
    xml += xmlGetElement(tagGuids.tagAuthor, this.author) + '\r\n';
    xml += xmlGetElement(tagGuids.tagKeywords, this.keywords) + '\r\n';
    xml += xmlGetElement(tagGuids.tagComments, this.comments) + '\r\n';
    xml += xmlGetElement(tagGuids.tagLastSavedBy, this.lastSavedBy) + '\r\n';
    xml += xmlGetElement(tagGuids.tagApplication, this.application) + '\r\n';
    xml += xmlGetElement(tagGuids.tagSecurity, this.security) + '\r\n';
    xml += xmlGetElement(tagGuids.tagOwner, this.owner) + '\r\n';
    xml += xmlGetElement(tagGuids.tagCopyright, this.copyright) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTrademarks, this.trademarks) + '\r\n';
    xml += xmlGetElement(tagGuids.tagNotes, this.notes) + '\r\n';
    if (this.compressionStyleID !== null) xml += xmlGetElement(tagGuids.tagCompressionStyleID, this.compressionStyleID) + '\r\n';
    if (this.compressionAlgorithmID !== null) xml += xmlGetElement(tagGuids.tagCompressionAlgorithmID, this.compressionAlgorithmID) + '\r\n';
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

    xml += '</tagContainer>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      recordType: 'Container',
      versionInfo: this.versionInfo ? Array.from(this.versionInfo) : null,
      fileName: this.fileName || '',
      creation: this.creation ? this.creation.toISOString() : null,
      lastSaved: this.lastSaved ? this.lastSaved.toISOString() : null,
      timesSaved: this.timesSaved,
      language: this.language || '',
      title: this.title || '',
      subject: this.subject || '',
      author: this.author || '',
      keywords: this.keywords || '',
      comments: this.comments || '',
      lastSavedBy: this.lastSavedBy || '',
      application: this.application || '',
      security: this.security || '',
      owner: this.owner || '',
      copyright: this.copyright || '',
      trademarks: this.trademarks || '',
      notes: this.notes || '',
      compressionStyleID: this.compressionStyleID,
      compressionAlgorithmID: this.compressionAlgorithmID,
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
  return [0, 0, 0, 0];
}

module.exports = Container;
