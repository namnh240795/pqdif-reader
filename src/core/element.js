'use strict';

const { PqdifInfo } = require('../info/info');
const { getTypeSize } = require('../constants/physicalTypes');

/**
 * CPQDIF_Element base class.
 * Ported from el_base.h/el_base.cpp
 */
class Element {
  constructor() {
    this.tag = null;            // 16-byte Buffer (GUID)
    this.physicalType = 0;
    this.record = null;         // reference to parent record
  }

  getElementType() {
    return -1; // Override in subclasses
  }

  getTag() {
    return this.tag;
  }

  setTag(tag) {
    this.tag = tag;
    if (this.record) {
      this.record.setChanged(true);
    }
  }

  getPhysicalType() {
    return this.physicalType;
  }

  setPhysicalType(type) {
    this.physicalType = type;
    if (this.record) {
      this.record.setChanged(true);
    }
  }

  getRecord() {
    return this.record;
  }

  setRecord(record) {
    this.record = record;
  }

  /**
   * Convert value to double.
   * @param {PqdifValue} value
   * @returns {number}
   */
  convertToDouble(value) {
    if (!value) return 0.0;
    return value.toDouble();
  }

  /**
   * Create PQDIFValue from double.
   * @param {number} doubleValue
   * @returns {PqdifValue}
   */
  convertFromDouble(doubleValue) {
    const { PqdifValue } = require('./pqdifValue');
    return PqdifValue.fromDouble(this.physicalType, doubleValue);
  }

  /**
   * Get number of bytes for the physical type.
   * @returns {number}
   */
  getNumBytesOfType() {
    return getTypeSize(this.physicalType);
  }
}

module.exports = { Element };
