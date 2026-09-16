'use strict';

const { Element } = require('./element');
const { PqdifValue } = require('./pqdifValue');
const { getTypeSize } = require('../constants/physicalTypes');

/**
 * CPQDIF_E_Vector - stores an array of values in a Buffer.
 * Ported from el_vect.h/el_vect.cpp
 */
class Vector extends Element {
  constructor() {
    super();
    this.data = null;   // Buffer backing store
    this.count = 0;
  }

  getElementType() {
    return 3; // ID_ELEMENT_TYPE_VECTOR
  }

  getCount() {
    return this.count;
  }

  /**
   * Set the count of elements, allocating/reallocating the buffer.
   * @param {number} count
   * @returns {boolean}
   */
  setCount(count) {
    const typeSize = getTypeSize(this.physicalType);
    if (typeSize <= 0) return false;
    const oldData = this.data;
    const oldByteLen = oldData ? oldData.length : 0;
    const newByteLen = count * typeSize;
    this.count = count;
    this.data = Buffer.alloc(newByteLen);
    // Preserve existing data up to the minimum of old and new sizes
    if (oldData && oldByteLen > 0) {
      oldData.copy(this.data, 0, 0, Math.min(oldByteLen, newByteLen));
    }
    return true;
  }

  SetCount(count) { return this.setCount(count); }

  /**
   * Get total size in bytes of the backing buffer.
   * @returns {number}
   */
  getBufferSize() {
    return this.data ? this.data.length : 0;
  }

  GetSizeBytes() { return this.getBufferSize(); }

  /**
   * Set physical type, reallocating buffer if type changed.
   * @param {number} type
   */
  setPhysicalType(type) {
    if (this.physicalType !== type) {
      const oldCount = this.data && this.physicalType
        ? this.data.length / getTypeSize(this.physicalType)
        : 0;
      this.physicalType = type;
      if (oldCount > 0) {
        this.setCount(oldCount);
      }
      if (this.record) {
        this.record.setChanged(true);
      }
    }
  }

  // ============================================================
  // Individual value access
  // ============================================================

  /**
   * Get a single value by index as a PqdifValue.
   * @param {number} index
   * @returns {PqdifValue|null}
   */
  getValue(index) {
    const typeSize = getTypeSize(this.physicalType);
    if (!this.data || index < 0 || index >= this.count) return null;
    return PqdifValue.readFromBuffer(this.data, index * typeSize, this.physicalType);
  }

  /**
   * Set a single value by index.
   * @param {number} index
   * @param {PqdifValue|*} value
   * @returns {boolean}
   */
  setValue(index, value) {
    const typeSize = getTypeSize(this.physicalType);
    if (!this.data || index < 0 || index >= this.count) return false;
    const offset = index * typeSize;
    if (value instanceof PqdifValue) {
      value.writeToBuffer(this.data, offset);
    } else {
      const pv = new PqdifValue(this.physicalType, value);
      pv.writeToBuffer(this.data, offset);
    }
    return true;
  }

  /**
   * Get a single value by index as a double.
   * @param {number} index
   * @returns {{ status: boolean, value: number }}
   */
  getValueAsDouble(index) {
    const pv = this.getValue(index);
    if (pv) {
      return { status: true, value: pv.toDouble() };
    }
    return { status: false, value: 0.0 };
  }

  /**
   * Set a single value by index from a double.
   * @param {number} index
   * @param {number} value
   * @returns {boolean}
   */
  setValueFromDouble(index, value) {
    const pv = PqdifValue.fromDouble(this.physicalType, value);
    return this.setValue(index, pv);
  }

  // ============================================================
  // Typed single-value setters/getters
  // ============================================================

  setValueINT1(idx, value) {
    if (this.physicalType !== 20) this.setPhysicalType(20);
    this.setValue(idx, value);
  }

  GetValueINT1(idx, out) {
    const r = this.getValueAsDouble(idx);
    if (r.status) out.value = r.value;
    return r.status;
  }

  SetValueINT1(idx, value) { this.setValueINT1(idx, value); }

  setValueINT2(idx, value) {
    if (this.physicalType !== 21) this.setPhysicalType(21);
    this.setValue(idx, value);
  }

  GetValueINT2(idx, out) {
    const r = this.getValueAsDouble(idx);
    if (r.status) out.value = r.value;
    return r.status;
  }

  SetValueINT2(idx, value) { this.setValueINT2(idx, value); }

  setValueINT4(idx, value) {
    if (this.physicalType !== 22) this.setPhysicalType(22);
    this.setValue(idx, value);
  }

  GetValueINT4(idx, out) {
    const r = this.getValueAsDouble(idx);
    if (r.status) out.value = r.value;
    return r.status;
  }

  SetValueINT4(idx, value) { this.setValueINT4(idx, value); }

  setValueUINT4(idx, value) {
    if (this.physicalType !== 32) this.setPhysicalType(32);
    this.setValue(idx, value);
  }

  GetValueUINT4(idx, out) {
    const r = this.getValueAsDouble(idx);
    if (r.status) out.value = r.value;
    return r.status;
  }

  SetValueUINT4(idx, value) { this.setValueUINT4(idx, value); }

  setValueREAL4(idx, value) {
    if (this.physicalType !== 40) this.setPhysicalType(40);
    this.setValue(idx, value);
  }

  GetValueREAL4(idx, out) {
    const r = this.getValueAsDouble(idx);
    if (r.status) out.value = r.value;
    return r.status;
  }

  SetValueREAL4(idx, value) { this.setValueREAL4(idx, value); }

  setValueREAL8(idx, value) {
    if (this.physicalType !== 41) this.setPhysicalType(41);
    this.setValue(idx, value);
  }

  GetValueREAL8(idx, out) {
    const r = this.getValueAsDouble(idx);
    if (r.status) out.value = r.value;
    return r.status;
  }

  SetValueREAL8(idx, value) { this.setValueREAL8(idx, value); }

  setValueTimeStamp(idx, value) {
    if (this.physicalType !== 50) this.setPhysicalType(50);
    this.setValue(idx, value);
  }

  GetValueTimeStamp(idx, out) {
    const pv = this.getValue(idx);
    if (pv) {
      out.value = pv.value;
      return true;
    }
    return false;
  }

  SetValueTimeStamp(idx, value) { this.setValueTimeStamp(idx, value); }

  // ============================================================
  // Bulk array operations
  // ============================================================

  /**
   * Set values from a typed array (matching C++ SetValuesINT1, etc.)
   * Uses direct Buffer operations for matching typed arrays.
   * @param {number} physicalType
   * @param {Array|TypedArray} values
   */
  setValuesArray(physicalType, values) {
    this.setPhysicalType(physicalType);
    this.setCount(values.length);
    if (!this.data || this.count === 0) return;

    // Fast path: if values is a TypedArray matching our physical type, copy directly
    const typeSize = getTypeSize(physicalType);
    const matchingCtor = _getTypedArrayCtor(physicalType);
    if (matchingCtor && values instanceof matchingCtor && values.length === this.count) {
      const targetBuffer = matchingCtor === Uint8Array || matchingCtor === Int8Array
        ? this.data
        : (matchingCtor === Float64Array || matchingCtor === Float32Array
            ? this.data
            : this.data);
      if (matchingCtor === Float64Array) {
        const view = new Float64Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Float32Array) {
        const view = new Float32Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Int32Array) {
        const view = new Int32Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Uint32Array) {
        const view = new Uint32Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Int16Array) {
        const view = new Int16Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Uint16Array) {
        const view = new Uint16Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Int8Array) {
        const view = new Int8Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      } else if (matchingCtor === Uint8Array) {
        const view = new Uint8Array(this.data.buffer, this.data.byteOffset, this.count);
        view.set(values);
      }
      return;
    }

    // Generic path: write directly to buffer at correct offsets
    for (let i = 0; i < values.length; i++) {
      const offset = i * typeSize;
      _writeValueToBuffer(this.data, offset, physicalType, values[i]);
    }
  }

  /**
   * Get values as a typed array.
   * Returns a view of the internal buffer for matching typed arrays.
   * @param {number} max
   * @returns {Array}
   */
  getValuesArray(max) {
    const count = Math.min(max, this.count);
    if (count === 0 || !this.data) return [];

    // Fast path: return a view of the internal buffer for matching types
    const matchingCtor = _getTypedArrayCtor(this.physicalType);
    if (matchingCtor) {
      if (matchingCtor === Float64Array) {
        return new Float64Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Float32Array) {
        return new Float32Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Int32Array) {
        return new Int32Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Uint32Array) {
        return new Uint32Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Int16Array) {
        return new Int16Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Uint16Array) {
        return new Uint16Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Int8Array) {
        return new Int8Array(this.data.buffer, this.data.byteOffset, count);
      } else if (matchingCtor === Uint8Array) {
        return new Uint8Array(this.data.buffer, this.data.byteOffset, count);
      }
    }

    // Generic path: read directly from buffer
    const typeSize = getTypeSize(this.physicalType);
    const result = [];
    for (let i = 0; i < count; i++) {
      const offset = i * typeSize;
      result.push(_readValueFromBuffer(this.data, offset, this.physicalType));
    }
    return result;
  }

  SetValuesINT1(values, count) { this.setValuesArray(20, values); }
  GetValuesINT1(array, max) { return this.getValuesArray(max); }

  SetValuesINT2(values, count) { this.setValuesArray(21, values); }
  GetValuesINT2(array, max) { return this.getValuesArray(max); }

  SetValuesINT4(values, count) { this.setValuesArray(22, values); }
  GetValuesINT4(array, max) { return this.getValuesArray(max); }

  SetValuesUINT4(values, count) { this.setValuesArray(32, values); }
  GetValuesUINT4(array, max) { return this.getValuesArray(max); }

  SetValuesREAL4(values, count) { this.setValuesArray(40, values); }
  GetValuesREAL4(array, max) { return this.getValuesArray(max); }

  SetValuesREAL8(values, count) { this.setValuesArray(41, values); }
  GetValuesREAL8(array, max) { return this.getValuesArray(max); }

  SetValuesTimeStamp(values, count) { this.setValuesArray(50, values); }
  GetValuesTimeStamp(array, max) { return this.getValuesArray(max); }

  // ============================================================
  // String operations (CHAR1 and CHAR2 vectors)
  // ============================================================

  /**
   * Set string value (matching C++ SetValues(const char* text))
   * @param {string} text
   * @returns {boolean}
   */
  setValues(text) {
    if (text === null || text === undefined) return false;

    if (this.physicalType === 10) { // CHAR1
      const countItems = text.length + 1; // include null terminator
      this.setCount(countItems);
      if (this.data) {
        // Copy string bytes
        this.data.write(text, 0, text.length, 'ascii');
        this.data[text.length] = 0; // null terminate
        return true;
      }
    } else if (this.physicalType === 11) { // CHAR2
      const countItems = text.length + 1;
      this.setCount(countItems);
      if (this.data) {
        for (let i = 0; i < text.length; i++) {
          this.data.writeInt16LE(text.charCodeAt(i), i * 2);
        }
        this.data.writeInt16LE(0, text.length * 2); // null terminate
        return true;
      }
    }
    return false;
  }

  SetValues(text) { return this.setValues(text); }

  /**
   * Get string value (matching C++ GetValues(string& text))
   * @param {Object} out - { value: string }
   * @returns {boolean}
   */
  getValues(out) {
    if (!this.data) return false;

    if (this.physicalType === 10) { // CHAR1
      // Read as ASCII string up to null terminator
      let end = this.count;
      while (end > 0 && this.data[end - 1] === 0) end--;
      out.value = this.data.toString('ascii', 0, end);
      return true;
    } else if (this.physicalType === 11) { // CHAR2
      let str = '';
      for (let i = 0; i < this.count - 1; i++) {
        const ch = this.data.readInt16LE(i * 2);
        if (ch === 0) break;
        str += String.fromCharCode(ch);
      }
      out.value = str;
      return true;
    }
    return false;
  }

  GetValues(out) {
    if (typeof out === 'string' || out === null) {
      // C++ style: string& text parameter
      const result = { value: '' };
      const status = this.getValues(result);
      return status ? result.value : '';
    }
    // Object style
    return this.getValues(out);
  }

  /**
   * Get string directly (convenience).
   * @returns {string}
   */
  getString() {
    const out = { value: '' };
    this.getValues(out);
    return out.value;
  }

  /**
   * Set string directly (convenience).
   * @param {string} str
   */
  setString(str) {
    if (this.physicalType === 0) {
      this.setPhysicalType(10); // Default to CHAR1
    }
    this.setValues(str);
  }

  // ============================================================
  // Raw data access
  // ============================================================

  getRawData() {
    return this.data;
  }

  setRawData(buf) {
    this.data = buf;
    const typeSize = getTypeSize(this.physicalType);
    this.count = typeSize > 0 ? buf.length / typeSize : 0;
  }

  GetRawData() { return this.getRawData(); }
}

/**
 * Returns the TypedArray constructor matching a physical type, or null if no direct match.
 * @param {number} physicalType
 * @returns {Function|null}
 * @private
 */
function _getTypedArrayCtor(physicalType) {
  switch (physicalType) {
    case 20: return Int8Array;      // INTEGER1
    case 21: return Int16Array;     // INTEGER2
    case 22: return Int32Array;     // INTEGER4
    case 32: return Uint32Array;    // UNS_INTEGER4
    case 40: return Float32Array;   // REAL4
    case 41: return Float64Array;   // REAL8
    case 3:  return Int32Array;     // BOOLEAN4 (4-byte)
    default: return null;
  }
}

/**
 * Write a value directly to a buffer at the given offset for the given physical type.
 * @param {Buffer} buf
 * @param {number} offset
 * @param {number} physicalType
 * @param {*} value
 * @private
 */
function _writeValueToBuffer(buf, offset, physicalType, value) {
  switch (physicalType) {
    case 20: // INTEGER1
      buf.writeInt8(value, offset);
      break;
    case 21: // INTEGER2
      buf.writeInt16LE(value, offset);
      break;
    case 22: // INTEGER4
      buf.writeInt32LE(value, offset);
      break;
    case 32: // UNS_INTEGER4
      buf.writeUInt32LE(value, offset);
      break;
    case 40: // REAL4
      buf.writeFloatLE(value, offset);
      break;
    case 41: // REAL8
      buf.writeDoubleLE(value, offset);
      break;
    case 3: // BOOLEAN4
      buf.writeInt32LE(value ? 1 : 0, offset);
      break;
    case 10: // CHAR1
      buf.writeUInt8(typeof value === 'string' ? value.charCodeAt(0) : value, offset);
      break;
    case 11: // CHAR2
      buf.writeInt16LE(typeof value === 'string' ? value.charCodeAt(0) : value, offset);
      break;
    case 30: // UNS_INTEGER1
      buf.writeUInt8(value, offset);
      break;
    case 31: // UNS_INTEGER2
      buf.writeUInt16LE(value, offset);
      break;
    case 42: // COMPLEX8 (two REAL4: real, imag)
      if (value && typeof value === 'object' && value.real !== undefined) {
        buf.writeFloatLE(value.real, offset);
        buf.writeFloatLE(value.imag, offset + 4);
      } else if (Array.isArray(value)) {
        buf.writeFloatLE(value[0] || 0, offset);
        buf.writeFloatLE(value[1] || 0, offset + 4);
      }
      break;
    case 43: // COMPLEX16 (two REAL8: real, imag)
      if (value && typeof value === 'object' && value.real !== undefined) {
        buf.writeDoubleLE(value.real, offset);
        buf.writeDoubleLE(value.imag, offset + 8);
      } else if (Array.isArray(value)) {
        buf.writeDoubleLE(value[0] || 0, offset);
        buf.writeDoubleLE(value[1] || 0, offset + 8);
      }
      break;
    case 50: // TIMESTAMPPQDIF (UINT4 day + REAL8 seconds)
      if (value instanceof Date) {
        // Convert PQDIF timestamp
        const { EXCEL_DAYCOUNT_ADJUST, SECONDS_PER_DAY } = require('../constants/physicalTypes');
        const dayCount = Math.floor(value.getTime() / (SECONDS_PER_DAY * 1000)) - EXCEL_DAYCOUNT_ADJUST;
        const fraction = (value.getTime() % (SECONDS_PER_DAY * 1000)) / (SECONDS_PER_DAY * 1000);
        buf.writeUInt32LE(dayCount, offset);
        buf.writeDoubleLE(fraction * SECONDS_PER_DAY, offset + 4);
      }
      break;
    case 60: // GUID (16 bytes)
      if (Buffer.isBuffer(value)) {
        value.copy(buf, offset, 0, Math.min(value.length, 16));
      }
      break;
    default:
      buf.writeUInt8(value, offset);
      break;
  }
}

/**
 * Read a value directly from a buffer at the given offset for the given physical type.
 * @param {Buffer} buf
 * @param {number} offset
 * @param {number} physicalType
 * @returns {*}
 * @private
 */
function _readValueFromBuffer(buf, offset, physicalType) {
  switch (physicalType) {
    case 20: return buf.readInt8(offset);
    case 21: return buf.readInt16LE(offset);
    case 22: return buf.readInt32LE(offset);
    case 32: return buf.readUInt32LE(offset);
    case 40: return buf.readFloatLE(offset);
    case 41: return buf.readDoubleLE(offset);
    case 3:  return buf.readInt32LE(offset) !== 0;
    case 10: return buf.readUInt8(offset);
    case 11: return buf.readInt16LE(offset);
    case 30: return buf.readUInt8(offset);
    case 31: return buf.readUInt16LE(offset);
    case 42: return { real: buf.readFloatLE(offset), imag: buf.readFloatLE(offset + 4) };
    case 43: return { real: buf.readDoubleLE(offset), imag: buf.readDoubleLE(offset + 8) };
    case 50: {
      const { EXCEL_DAYCOUNT_ADJUST, SECONDS_PER_DAY } = require('../constants/physicalTypes');
      const dayCount = buf.readUInt32LE(offset);
      const seconds = buf.readDoubleLE(offset + 4);
      const ts = (dayCount + EXCEL_DAYCOUNT_ADJUST) * SECONDS_PER_DAY * 1000 + seconds * 1000;
      return new Date(ts);
    }
    case 60: return buf.subarray(offset, offset + 16);
    default: return buf.readUInt8(offset);
  }
}

module.exports = { Vector };
