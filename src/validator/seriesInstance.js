'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement, isNumeric, fixNull } = require('./utilities');
const { ID_PHYS_TYPE_REAL4, ID_PHYS_TYPE_REAL8, ID_PHYS_TYPE_UNS_INTEGER4,
  ID_PHYS_TYPE_UNS_INTEGER2, ID_PHYS_TYPE_UNS_INTEGER1,
  ID_PHYS_TYPE_INTEGER4, ID_PHYS_TYPE_INTEGER2, ID_PHYS_TYPE_INTEGER1,
  ID_PHYS_TYPE_BOOLEAN1, ID_PHYS_TYPE_BOOLEAN2, ID_PHYS_TYPE_BOOLEAN4,
  ID_PHYS_TYPE_TIMESTAMPPQDIF } = require('../constants/physicalTypes');
const { ID_ELEMENT_TYPE_VECTOR } = require('../constants/elementTypes');

/**
 * Series instance - ported from OneSeriesInstance.cs + GetSeriesValues.cs.
 */
class SeriesInstance {
  constructor() {
    /** @type {number|null} Series base quantity */
    this.seriesBaseQuantity = null;

    /** @type {number|null} Series scale */
    this.seriesScale = null;

    /** @type {number|null} Series offset */
    this.seriesOffset = null;

    /** @type {number|null} Share channel index */
    this.seriesShareChannelIdx = null;

    /** @type {number|null} Share series index */
    this.seriesShareSeriesIdx = null;

    /** @type {Array|null} Series values (typed array or regular array) */
    this.seriesValues = null;

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {SeriesInstance}
   */
  static fromCollection(coll) {
    const si = new SeriesInstance();
    if (!coll) return si;

    let scalePhysType = null;
    let offsetPhysType = null;
    let valuesPhysType = null;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagSeriesBaseQuantity)) {
          si.seriesBaseQuantity = _getDoubleFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesScale)) {
          si.seriesScale = _getDoubleFromElement(el);
          if (typeof el.getPhysicalType === 'function') {
            scalePhysType = el.getPhysicalType();
          }
        } else if (guidEquals(tag, tagGuids.tagSeriesOffset)) {
          si.seriesOffset = _getDoubleFromElement(el);
          if (typeof el.getPhysicalType === 'function') {
            offsetPhysType = el.getPhysicalType();
          }
        } else if (guidEquals(tag, tagGuids.tagSeriesShareChannelIdx)) {
          si.seriesShareChannelIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesShareSeriesIdx)) {
          si.seriesShareSeriesIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesValues)) {
          si.seriesValues = _getArrayFromElement(el);
          if (typeof el.getPhysicalType === 'function') {
            valuesPhysType = el.getPhysicalType();
          }
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          si.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        si.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    // Validate: seriesValues required unless shared
    const missingValues = !si.seriesValues;
    const missingShare = si.seriesShareChannelIdx === null || si.seriesShareSeriesIdx === null;
    if (missingValues && missingShare) {
      si.loggerCompliance.log('tagSeriesValues, tagSeriesShareChannelIdx, and tagSeriesShareSeriesIdx are missing.', LogLevels.Error);
    } else if (missingValues && si.seriesShareChannelIdx === null) {
      si.loggerCompliance.log('tagSeriesValues and tagSeriesShareChannelIdx are missing.', LogLevels.Error);
    } else if (missingValues && si.seriesShareSeriesIdx === null) {
      si.loggerCompliance.log('tagSeriesValues and tagSeriesShareSeriesIdx are missing.', LogLevels.Error);
    }

    // Physical type mismatch warnings for scale/offset vs values
    if (valuesPhysType !== null) {
      if (scalePhysType !== null && scalePhysType !== valuesPhysType) {
        si.loggerCompliance.log('Physical type of tagSeriesScale does not match tagSeriesValues.', LogLevels.Warning);
      }
      if (offsetPhysType !== null && offsetPhysType !== valuesPhysType) {
        si.loggerCompliance.log('Physical type of tagSeriesOffset does not match tagSeriesValues.', LogLevels.Warning);
      }
    }

    return si;
  }

  /**
   * Get series values as an array of doubles, applying scale/offset if needed.
   * Ported from GetSeriesValues(uint StorageMethodID).
   * @param {number} storageMethodID
   * @param {number} [idSeriesMethodIncrement=0x1000000] - ID_SERIES_METHOD_INCREMENT
   * @param {number} [idSeriesMethodScaled=0x2000000] - ID_SERIES_METHOD_SCALED
   * @returns {number[]|null}
   */
  getSeriesValues(storageMethodID, idSeriesMethodIncrement, idSeriesMethodScaled) {
    idSeriesMethodIncrement = idSeriesMethodIncrement || 0x1000000;
    idSeriesMethodScaled = idSeriesMethodScaled || 0x2000000;

    if (!this.seriesValues) return null;

    let retValues = null;
    let zeroValues = false;

    const values = this.seriesValues;
    const len = values.length;

    // Convert to double array
    if (Array.isArray(values)) {
      retValues = new Array(len);
      for (let i = 0; i < len; i++) {
        retValues[i] = Number(values[i]);
      }
    } else if (values instanceof Float64Array) {
      retValues = Array.from(values);
    } else if (values instanceof Float32Array) {
      retValues = new Array(len);
      for (let i = 0; i < len; i++) retValues[i] = values[i];
    } else if (values instanceof Int32Array) {
      retValues = new Array(len);
      for (let i = 0; i < len; i++) retValues[i] = values[i];
    } else if (values instanceof Uint32Array) {
      retValues = new Array(len);
      for (let i = 0; i < len; i++) retValues[i] = values[i];
    } else if (values instanceof Int16Array) {
      retValues = new Array(len);
      for (let i = 0; i < len; i++) retValues[i] = values[i];
    } else if (values instanceof Uint16Array) {
      retValues = new Array(len);
      for (let i = 0; i < len; i++) retValues[i] = values[i];
    } else if (Buffer.isBuffer(values)) {
      // Assume UINT4 if buffer
      retValues = new Array(Math.floor(len / 4));
      for (let i = 0; i < retValues.length; i++) {
        retValues[i] = values.readUInt32LE(i * 4);
      }
    } else {
      // Unknown type - zero values
      zeroValues = true;
      retValues = new Array(len);
      retValues.fill(0);
    }

    // Handle increment storage method
    if ((storageMethodID & idSeriesMethodIncrement) === idSeriesMethodIncrement && retValues.length > 0) {
      const incrementedSamples = [];
      const rateCount = Math.floor(retValues[0]);
      for (let rateIndex = 0; rateIndex < rateCount; rateIndex++) {
        const pointCountIdx = rateIndex * 2 + 1;
        const rateIdx = rateIndex * 2 + 2;
        if (retValues.length > rateIdx) {
          const pointCount = Math.floor(retValues[pointCountIdx]);
          const rate = retValues[rateIdx];
          for (let pointIndex = 0; pointIndex < pointCount; pointIndex++) {
            if (incrementedSamples.length > 0) {
              incrementedSamples.push(incrementedSamples[incrementedSamples.length - 1] + rate);
            } else {
              incrementedSamples.push(0);
            }
          }
        }
      }
      if (incrementedSamples.length > 0) retValues = incrementedSamples;
    }

    // Apply scale and offset
    const scaled = (storageMethodID & idSeriesMethodScaled) === idSeriesMethodScaled;
    if (scaled && !zeroValues && !(values instanceof Array && values.length > 0 && typeof values[0] === 'boolean')) {
      const scale = Number(fixNull(this.seriesScale, 1));
      const offset = Number(fixNull(this.seriesOffset, 0));
      for (let i = 0; i < retValues.length; i++) {
        retValues[i] = (retValues[i] * scale) + offset;
      }
    }

    return retValues;
  }

  /**
   * Get the base type name of the series values.
   * @returns {string|null}
   */
  seriesValuesBaseType() {
    if (!this.seriesValues) return null;
    const v = this.seriesValues;
    if (Array.isArray(v)) {
      if (v.length > 0) {
        if (typeof v[0] === 'boolean') return 'bool';
        if (typeof v[0] === 'number') return 'double';
      }
      return null;
    }
    if (v instanceof Float64Array) return 'double';
    if (v instanceof Float32Array) return 'float';
    if (v instanceof Int32Array) return 'int';
    if (v instanceof Uint32Array) return 'uint';
    if (v instanceof Int16Array) return 'short';
    if (v instanceof Uint16Array) return 'ushort';
    return null;
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @param {number|null} maxSeriesValues
   * @returns {string}
   */
  getXML(applicationLog, complianceLog, maxSeriesValues) {
    let xml = '<tagOneSeriesInstance>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    if (this.seriesBaseQuantity !== null) xml += xmlGetElement(tagGuids.tagSeriesBaseQuantity, this.seriesBaseQuantity) + '\r\n';
    if (this.seriesScale !== null) xml += xmlGetElement(tagGuids.tagSeriesScale, this.seriesScale) + '\r\n';
    if (this.seriesOffset !== null) xml += xmlGetElement(tagGuids.tagSeriesOffset, this.seriesOffset) + '\r\n';
    if (this.seriesShareChannelIdx !== null) xml += xmlGetElement(tagGuids.tagSeriesShareChannelIdx, this.seriesShareChannelIdx) + '\r\n';
    if (this.seriesShareSeriesIdx !== null) xml += xmlGetElement(tagGuids.tagSeriesShareSeriesIdx, this.seriesShareSeriesIdx) + '\r\n';
    if (this.seriesValues) xml += xmlGetElement(tagGuids.tagSeriesValues, this.seriesValues, maxSeriesValues) + '\r\n';

    xml += '</tagOneSeriesInstance>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @param {number|null} maxSeriesValues
   * @returns {Object}
   */
  getJSON(maxSeriesValues) {
    let values = null;
    if (this.seriesValues) {
      const arr = Array.isArray(this.seriesValues) ? this.seriesValues : Array.from(this.seriesValues);
      if (maxSeriesValues && arr.length > maxSeriesValues) {
        values = arr.slice(0, maxSeriesValues);
        values.push('...');
      } else {
        values = arr;
      }
    }
    return {
      seriesValues: values,
      seriesBaseQuantity: this.seriesBaseQuantity,
      seriesScale: this.seriesScale,
      seriesOffset: this.seriesOffset,
      seriesShareChannelIdx: this.seriesShareChannelIdx,
      seriesShareSeriesIdx: this.seriesShareSeriesIdx
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

function _getArrayFromElement(el) {
  if (el && typeof el.getValuesArray === 'function') {
    return el.getValuesArray(el.getCount ? el.getCount() : 0);
  }
  if (el && el.getRawData) {
    return el.getRawData();
  }
  return null;
}

module.exports = SeriesInstance;
