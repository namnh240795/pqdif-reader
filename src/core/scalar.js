'use strict';

const { Element } = require('./element');
const { PqdifValue } = require('./pqdifValue');
const {
  ID_PHYS_TYPE_UNS_INTEGER2, ID_PHYS_TYPE_INTEGER2,
  ID_PHYS_TYPE_UNS_INTEGER4, ID_PHYS_TYPE_INTEGER4,
  ID_PHYS_TYPE_BOOLEAN4, ID_PHYS_TYPE_REAL4, ID_PHYS_TYPE_REAL8,
  ID_PHYS_TYPE_COMPLEX8, ID_PHYS_TYPE_COMPLEX16,
  ID_PHYS_TYPE_GUID, ID_PHYS_TYPE_TIMESTAMPPQDIF,
  getTypeSize,
} = require('../constants/physicalTypes');

/**
 * CPQDIF_E_Scalar - stores a single value.
 * Ported from el_scal.h/el_scal.cpp
 */
class Scalar extends Element {
  constructor() {
    super();
    this.value = null; // PqdifValue
  }

  getElementType() {
    return 2; // ID_ELEMENT_TYPE_SCALAR
  }

  /**
   * Get value as a PqdifValue.
   * @param {Object} out - { typePhysical, value }
   * @returns {boolean}
   */
  getValue(out) {
    if (this.physicalType !== 0 && this.value !== null) {
      out.typePhysical = this.physicalType;
      out.value = this.value;
      return true;
    }
    return false;
  }

  /**
   * Set value with explicit physical type.
   * @param {number} typePhysical
   * @param {PqdifValue|*} value
   * @returns {boolean}
   */
  setValue(typePhysical, value) {
    const iSize = getTypeSize(typePhysical);
    if (iSize > 0) {
      this.physicalType = typePhysical;
      if (value instanceof PqdifValue) {
        this.value = value.clone();
      } else {
        this.value = new PqdifValue(typePhysical, value);
      }
      if (this.record) {
        this.record.setChanged(true);
      }
      return true;
    }
    return false;
  }

  // ============================================================
  // Type-specific setters
  // ============================================================

  setValueUINT2(value) {
    this.physicalType = ID_PHYS_TYPE_UNS_INTEGER2;
    this.value = new PqdifValue(ID_PHYS_TYPE_UNS_INTEGER2, value);
    return true;
  }

  SetValueUINT2(value) { return this.setValueUINT2(value); }

  setValueINT2(value) {
    this.physicalType = ID_PHYS_TYPE_INTEGER2;
    this.value = new PqdifValue(ID_PHYS_TYPE_INTEGER2, value);
    return true;
  }

  SetValueINT2(value) { return this.setValueINT2(value); }

  setValueUINT4(value) {
    this.physicalType = ID_PHYS_TYPE_UNS_INTEGER4;
    this.value = new PqdifValue(ID_PHYS_TYPE_UNS_INTEGER4, value);
    return true;
  }

  SetValueUINT4(value) { return this.setValueUINT4(value); }

  setValueINT4(value) {
    this.physicalType = ID_PHYS_TYPE_INTEGER4;
    this.value = new PqdifValue(ID_PHYS_TYPE_INTEGER4, value);
    return true;
  }

  SetValueINT4(value) { return this.setValueINT4(value); }

  setValueBOOL4(value) {
    this.physicalType = ID_PHYS_TYPE_BOOLEAN4;
    this.value = new PqdifValue(ID_PHYS_TYPE_BOOLEAN4, value ? 1 : 0);
    return true;
  }

  SetValueBOOL4(value) { return this.setValueBOOL4(value); }

  setValueREAL4(value) {
    this.physicalType = ID_PHYS_TYPE_REAL4;
    this.value = new PqdifValue(ID_PHYS_TYPE_REAL4, value);
    return true;
  }

  SetValueREAL4(value) { return this.setValueREAL4(value); }

  setValueREAL8(value) {
    this.physicalType = ID_PHYS_TYPE_REAL8;
    this.value = new PqdifValue(ID_PHYS_TYPE_REAL8, value);
    return true;
  }

  SetValueREAL8(value) { return this.setValueREAL8(value); }

  setValueCOMPLEX8(value) {
    this.physicalType = ID_PHYS_TYPE_COMPLEX8;
    this.value = new PqdifValue(ID_PHYS_TYPE_COMPLEX8, value);
    return true;
  }

  SetValueCOMPLEX8(value) { return this.setValueCOMPLEX8(value); }

  setValueCOMPLEX16(value) {
    this.physicalType = ID_PHYS_TYPE_COMPLEX16;
    this.value = new PqdifValue(ID_PHYS_TYPE_COMPLEX16, value);
    return true;
  }

  SetValueCOMPLEX16(value) { return this.setValueCOMPLEX16(value); }

  setValueGUID(value) {
    this.physicalType = ID_PHYS_TYPE_GUID;
    this.value = new PqdifValue(ID_PHYS_TYPE_GUID, value);
    return true;
  }

  SetValueGUID(value) { return this.setValueGUID(value); }

  setValueTimeStamp(value) {
    this.physicalType = ID_PHYS_TYPE_TIMESTAMPPQDIF;
    this.value = new PqdifValue(ID_PHYS_TYPE_TIMESTAMPPQDIF, value);
    return true;
  }

  SetValueTimeStamp(value) { return this.setValueTimeStamp(value); }

  // ============================================================
  // Type-specific getters
  // ============================================================

  getValueUINT2() {
    if (this.physicalType === ID_PHYS_TYPE_UNS_INTEGER2 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: 0 };
  }

  GetValueUINT2(out) {
    const r = this.getValueUINT2();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueINT2() {
    if (this.physicalType === ID_PHYS_TYPE_INTEGER2 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: 0 };
  }

  GetValueINT2(out) {
    const r = this.getValueINT2();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueUINT4() {
    if (this.physicalType === ID_PHYS_TYPE_UNS_INTEGER4 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: 0 };
  }

  GetValueUINT4(out) {
    const r = this.getValueUINT4();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueINT4() {
    if (this.physicalType === ID_PHYS_TYPE_INTEGER4 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: 0 };
  }

  GetValueINT4(out) {
    const r = this.getValueINT4();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueBOOL4() {
    if (this.physicalType === ID_PHYS_TYPE_BOOLEAN4 && this.value) {
      return { status: true, value: this.value.value !== 0 };
    }
    return { status: false, value: false };
  }

  GetValueBOOL4(out) {
    const r = this.getValueBOOL4();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueREAL4() {
    if (this.physicalType === ID_PHYS_TYPE_REAL4 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: 0.0 };
  }

  GetValueREAL4(out) {
    const r = this.getValueREAL4();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueREAL8() {
    if (this.physicalType === ID_PHYS_TYPE_REAL8 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: 0.0 };
  }

  GetValueREAL8(out) {
    const r = this.getValueREAL8();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueCOMPLEX8() {
    if (this.physicalType === ID_PHYS_TYPE_COMPLEX8 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: null };
  }

  GetValueCOMPLEX8(out) {
    const r = this.getValueCOMPLEX8();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueCOMPLEX16() {
    if (this.physicalType === ID_PHYS_TYPE_COMPLEX16 && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: null };
  }

  GetValueCOMPLEX16(out) {
    const r = this.getValueCOMPLEX16();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueGUID() {
    if (this.physicalType === ID_PHYS_TYPE_GUID && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: null };
  }

  GetValueGUID(out) {
    const r = this.getValueGUID();
    if (r.status) out.value = r.value;
    return r.status;
  }

  getValueTimeStamp() {
    if (this.physicalType === ID_PHYS_TYPE_TIMESTAMPPQDIF && this.value) {
      return { status: true, value: this.value.value };
    }
    return { status: false, value: { day: 0, sec: 0.0 } };
  }

  GetValueTimeStamp(out) {
    const r = this.getValueTimeStamp();
    if (r.status) out.value = r.value;
    return r.status;
  }
}

module.exports = { Scalar };
