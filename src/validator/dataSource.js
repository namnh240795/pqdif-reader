'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement, pqdifTimestampToDate } = require('./utilities');
const ChannelDefinition = require('./channelDefinition');
const CustomSourceInfo = require('./customSourceInfo');

/**
 * Data source record wrapper - ported from RecDataSource.cs.
 */
class DataSource {
  constructor() {
    /** @type {Buffer} Data source type GUID */
    this.dataSourceTypeID = null;

    /** @type {Buffer} Vendor GUID */
    this.vendorID = null;

    /** @type {Buffer} Equipment GUID */
    this.equipmentID = null;

    /** @type {CustomSourceInfo} Custom source info */
    this.customSourceInfo = new CustomSourceInfo();

    /** @type {string} Serial number */
    this.serialNumberDS = '';

    /** @type {string} Version */
    this.versionDS = '';

    /** @type {string} Name */
    this.nameDS = '';

    /** @type {string} Owner */
    this.ownerDS = '';

    /** @type {string} Location */
    this.locationDS = '';

    /** @type {string} Time zone */
    this.timeZoneDS = '';

    /** @type {number[]} Coordinates (longitude, latitude) */
    this.coordinatesDS = [0, 0];

    /** @type {ChannelDefinition[]} Channel definitions */
    this.channelDefns = [];

    /** @type {Date} Effective time */
    this.effective = new Date(0);

    /** @type {number} Latitude */
    this.latitude = 0;

    /** @type {number} Longitude */
    this.longitude = 0;

    /** @type {string} Comments */
    this.comments = '';

    /** @type {number|null} UTC to LST offset in seconds */
    this.utcToLST = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {DataSource}
   */
  static fromCollection(coll) {
    const ds = new DataSource();
    if (!coll) return ds;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagCoordinatesDS)) {
          ds.coordinatesDS = _getUintArrayFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagDataSourceTypeID)) {
          ds.dataSourceTypeID = _getGuidFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagVendorID)) {
          ds.vendorID = _getGuidFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagEquipmentID)) {
          ds.equipmentID = _getGuidFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSerialNumberDS)) {
          ds.serialNumberDS = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagVersionDS)) {
          ds.versionDS = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagNameDS)) {
          ds.nameDS = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagOwnerDS)) {
          ds.ownerDS = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLocationDS)) {
          ds.locationDS = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagTimeZoneDS)) {
          ds.timeZoneDS = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagEffective)) {
          ds.effective = _getDateFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLatitude)) {
          ds.latitude = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagLongitude)) {
          ds.longitude = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagComments)) {
          ds.comments = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagUTCtoLST)) {
          ds.utcToLST = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagChannelDefns)) {
          // Channel definitions sub-collection
          if (typeof el.getCount === 'function') {
            const chCount = el.getCount();
            if (chCount === 0) {
              ds.loggerCompliance.log('tagChannelDefns must have at least one tagOneChannelDefn record.', LogLevels.Error);
            }
            for (let j = 0; j < chCount; j++) {
              const chEl = el.getElement(j);
              if (chEl && typeof chEl.getCount === 'function') {
                const chDefn = ChannelDefinition.fromCollection(chEl);
                ds.channelDefns.push(chDefn);
              }
            }
          }
        } else if (guidEquals(tag, tagGuids.tagCustomSourceInfo)) {
          // Custom source info sub-collection
          if (typeof el.getCount === 'function') {
            ds.customSourceInfo = CustomSourceInfo.fromCollection(el);
          }
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          ds.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        ds.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    // Check required tags
    if (ds.channelDefns.length === 0) {
      ds.loggerCompliance.log('Required tagChannelDefns is Missing.', LogLevels.Error);
    }

    return ds;
  }

  /**
   * Get the nominal voltage for this data source.
   * @returns {number|null}
   */
  getNominalVoltage() {
    const Peak2Rms = 0.70710678118654757;
    const idQmVoltage = require('../constants/idIntegers').ID_QM_VOLTAGE;
    const idQcInstantaneous = require('../constants/idGuids').ID_QC_INSTANTANEOUS;
    const idQcRms = require('../constants/idGuids').ID_QC_RMS;
    const idValueTypeTime = require('../constants/idGuids').ID_SERIES_VALUE_TYPE_TIME;

    for (const chDefn of this.channelDefns) {
      if (chDefn.quantityMeasuredID === idQmVoltage) {
        for (const sd of chDefn.seriesDefns) {
          if ((guidEquals(sd.quantityCharacteristicID, idQcInstantaneous) || guidEquals(sd.quantityCharacteristicID, idQcRms)) &&
              (sd.valueTypeID && !guidEquals(sd.valueTypeID, idValueTypeTime))) {
            if (sd.seriesNominalQuantity !== null) {
              if (guidEquals(sd.quantityCharacteristicID, idQcInstantaneous)) {
                return sd.seriesNominalQuantity * Peak2Rms;
              } else {
                return sd.seriesNominalQuantity;
              }
            }
          }
        }
      }
    }
    return null;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagRecDataSource>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    if (this.dataSourceTypeID) xml += xmlGetElement(tagGuids.tagDataSourceTypeID, this.dataSourceTypeID) + '\r\n';
    if (this.vendorID) xml += xmlGetElement(tagGuids.tagVendorID, this.vendorID) + '\r\n';
    if (this.equipmentID) xml += xmlGetElement(tagGuids.tagEquipmentID, this.equipmentID) + '\r\n';
    xml += this.customSourceInfo.getXML(applicationLog, complianceLog);
    xml += xmlGetElement(tagGuids.tagSerialNumberDS, this.serialNumberDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagVersionDS, this.versionDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagNameDS, this.nameDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagOwnerDS, this.ownerDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagLocationDS, this.locationDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagTimeZoneDS, this.timeZoneDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagCoordinatesDS, this.coordinatesDS) + '\r\n';
    xml += xmlGetElement(tagGuids.tagComments, this.comments) + '\r\n';
    if (this.utcToLST !== null) xml += xmlGetElement(tagGuids.tagUTCtoLST, this.utcToLST) + '\r\n';

    xml += '<tagChannelDefns>\r\n';
    for (const ch of this.channelDefns) {
      xml += ch.getXML(applicationLog, complianceLog);
    }
    xml += '</tagChannelDefns>\r\n';

    xml += xmlGetElement(tagGuids.tagLatitude, this.latitude) + '\r\n';
    xml += xmlGetElement(tagGuids.tagLongitude, this.longitude) + '\r\n';

    xml += '</tagRecDataSource>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      recordType: 'DataSource',
      dataSourceTypeID: this.dataSourceTypeID ? this.dataSourceTypeID.toString('hex') : null,
      vendorID: this.vendorID ? this.vendorID.toString('hex') : null,
      equipmentID: this.equipmentID ? this.equipmentID.toString('hex') : null,
      serialNumber: this.serialNumberDS || '',
      version: this.versionDS || '',
      name: this.nameDS || '',
      owner: this.ownerDS || '',
      location: this.locationDS || '',
      timeZone: this.timeZoneDS || '',
      latitude: this.latitude,
      longitude: this.longitude,
      comments: this.comments || '',
      utcToLST: this.utcToLST,
      effective: this.effective ? this.effective.toISOString() : null,
      channelDefinitions: this.channelDefns.map(ch => ch.getJSON()),
      customSourceInfo: this.customSourceInfo ? this.customSourceInfo.getJSON() : null
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
  return [0, 0];
}

module.exports = DataSource;
