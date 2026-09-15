'use strict';

const { getTypeSize, ID_PHYS_TYPE_TIMESTAMPPQDIF, ID_PHYS_TYPE_GUID } = require('../constants/physicalTypes');

/**
 * PQDIFValue - replaces the C++ PQDIFValue union.
 * Stores values as a { physicalType, value } pair.
 */
class PqdifValue {
  constructor(physicalType = 0, value = null) {
    this.physicalType = physicalType;
    this.value = value;
  }

  // Static factory methods
  static fromBool(v) {
    return new PqdifValue(3, v ? 1 : 0); // BOOLEAN4
  }

  static fromInt(v) {
    return new PqdifValue(22, v); // INTEGER4
  }

  static fromUint(v) {
    return new PqdifValue(32, v); // UNS_INTEGER4
  }

  static fromReal4(v) {
    return new PqdifValue(40, v);
  }

  static fromReal8(v) {
    return new PqdifValue(41, v);
  }

  static fromGuid(buf) {
    return new PqdifValue(60, buf);
  }

  static fromTimestamp(day, sec) {
    return new PqdifValue(50, { day, sec });
  }

  /**
   * Read a value from a buffer at a given offset based on physical type.
   * @param {Buffer} buffer
   * @param {number} offset
   * @param {number} physicalType
   * @returns {PqdifValue}
   */
  static readFromBuffer(buffer, offset, physicalType) {
    switch (physicalType) {
      case 1: // BOOLEAN1
        return new PqdifValue(physicalType, buffer.readInt8(offset));
      case 2: // BOOLEAN2
        return new PqdifValue(physicalType, buffer.readInt16LE(offset));
      case 3: // BOOLEAN4
        return new PqdifValue(physicalType, buffer.readInt32LE(offset));

      case 10: // CHAR1
        return new PqdifValue(physicalType, buffer.readInt8(offset));
      case 11: // CHAR2
        return new PqdifValue(physicalType, buffer.readInt16LE(offset));

      case 20: // INTEGER1
        return new PqdifValue(physicalType, buffer.readInt8(offset));
      case 21: // INTEGER2
        return new PqdifValue(physicalType, buffer.readInt16LE(offset));
      case 22: // INTEGER4
        return new PqdifValue(physicalType, buffer.readInt32LE(offset));

      case 30: // UNS_INTEGER1
        return new PqdifValue(physicalType, buffer.readUInt8(offset));
      case 31: // UNS_INTEGER2
        return new PqdifValue(physicalType, buffer.readUInt16LE(offset));
      case 32: // UNS_INTEGER4
        return new PqdifValue(physicalType, buffer.readUInt32LE(offset));

      case 40: // REAL4
        return new PqdifValue(physicalType, buffer.readFloatLE(offset));
      case 41: // REAL8
        return new PqdifValue(physicalType, buffer.readDoubleLE(offset));

      case 42: // COMPLEX8 (2x float LE)
        return new PqdifValue(physicalType, {
          real: buffer.readFloatLE(offset),
          image: buffer.readFloatLE(offset + 4),
        });

      case 43: // COMPLEX16 (2x double LE)
        return new PqdifValue(physicalType, {
          real: buffer.readDoubleLE(offset),
          image: buffer.readDoubleLE(offset + 8),
        });

      case 50: // TIMESTAMPPQDIF (UINT4 day + REAL8 sec)
        return new PqdifValue(physicalType, {
          day: buffer.readUInt32LE(offset),
          sec: buffer.readDoubleLE(offset + 4),
        });

      case 60: // GUID (16 bytes)
        return new PqdifValue(physicalType, Buffer.from(buffer.subarray(offset, offset + 16)));

      default:
        return new PqdifValue(physicalType, null);
    }
  }

  /**
   * Write this value to a buffer at a given offset.
   * @param {Buffer} buffer
   * @param {number} offset
   */
  writeToBuffer(buffer, offset) {
    switch (this.physicalType) {
      case 1: // BOOLEAN1
        buffer.writeInt8(this.value, offset);
        break;
      case 2: // BOOLEAN2
        buffer.writeInt16LE(this.value, offset);
        break;
      case 3: // BOOLEAN4
        buffer.writeInt32LE(this.value, offset);
        break;

      case 10: // CHAR1
        buffer.writeInt8(this.value, offset);
        break;
      case 11: // CHAR2
        buffer.writeInt16LE(this.value, offset);
        break;

      case 20: // INTEGER1
        buffer.writeInt8(this.value, offset);
        break;
      case 21: // INTEGER2
        buffer.writeInt16LE(this.value, offset);
        break;
      case 22: // INTEGER4
        buffer.writeInt32LE(this.value, offset);
        break;

      case 30: // UNS_INTEGER1
        buffer.writeUInt8(this.value, offset);
        break;
      case 31: // UNS_INTEGER2
        buffer.writeUInt16LE(this.value, offset);
        break;
      case 32: // UNS_INTEGER4
        buffer.writeUInt32LE(this.value, offset);
        break;

      case 40: // REAL4
        buffer.writeFloatLE(this.value, offset);
        break;
      case 41: // REAL8
        buffer.writeDoubleLE(this.value, offset);
        break;

      case 42: // COMPLEX8
        buffer.writeFloatLE(this.value.real, offset);
        buffer.writeFloatLE(this.value.image, offset + 4);
        break;

      case 43: // COMPLEX16
        buffer.writeDoubleLE(this.value.real, offset);
        buffer.writeDoubleLE(this.value.image, offset + 8);
        break;

      case 50: // TIMESTAMPPQDIF
        buffer.writeUInt32LE(this.value.day, offset);
        buffer.writeDoubleLE(this.value.sec, offset + 4);
        break;

      case 60: // GUID
        if (Buffer.isBuffer(this.value)) {
          this.value.copy(buffer, offset);
        }
        break;
    }
  }

  /**
   * Deep clone this value.
   * @returns {PqdifValue}
   */
  clone() {
    if (this.physicalType === 60 && Buffer.isBuffer(this.value)) {
      return new PqdifValue(this.physicalType, Buffer.from(this.value));
    }
    if (this.physicalType === 50 && this.value && typeof this.value === 'object') {
      return new PqdifValue(this.physicalType, { day: this.value.day, sec: this.value.sec });
    }
    if (this.physicalType === 42 && this.value && typeof this.value === 'object') {
      return new PqdifValue(this.physicalType, { real: this.value.real, image: this.value.image });
    }
    if (this.physicalType === 43 && this.value && typeof this.value === 'object') {
      return new PqdifValue(this.physicalType, { real: this.value.real, image: this.value.image });
    }
    return new PqdifValue(this.physicalType, this.value);
  }

  /**
   * Convert value to double (matching C++ convertToDouble).
   * @returns {number}
   */
  toDouble() {
    switch (this.physicalType) {
      case 1:  return this.value;           // BOOLEAN1
      case 2:  return this.value;           // BOOLEAN2
      case 3:  return this.value;           // BOOLEAN4
      case 10: return this.value;           // CHAR1
      case 11: return this.value;           // CHAR2
      case 20: return this.value;           // INTEGER1
      case 21: return this.value;           // INTEGER2
      case 22: return this.value;           // INTEGER4
      case 30: return this.value;           // UNS_INTEGER1
      case 31: return this.value;           // UNS_INTEGER2
      case 32: return this.value;           // UNS_INTEGER4
      case 40: return this.value;           // REAL4
      case 41: return this.value;           // REAL8
      case 50:                              // TIMESTAMPPQDIF
        return this.value.day + (this.value.sec / 86400);
      default:
        return 0.0;
    }
  }

  /**
   * Create a PqdifValue from a double (matching C++ convertFromDouble).
   * @param {number} physicalType
   * @param {number} valueSource
   * @returns {PqdifValue}
   */
  static fromDouble(physicalType, valueSource) {
    switch (physicalType) {
      case 1:  return new PqdifValue(physicalType, valueSource ? 1 : 0);
      case 2:  return new PqdifValue(physicalType, valueSource ? 1 : 0);
      case 3:  return new PqdifValue(physicalType, valueSource ? 1 : 0);
      case 10: return new PqdifValue(physicalType, valueSource);
      case 20: return new PqdifValue(physicalType, valueSource);
      case 30: return new PqdifValue(physicalType, valueSource);
      case 11: return new PqdifValue(physicalType, valueSource);
      case 21: return new PqdifValue(physicalType, valueSource);
      case 31: return new PqdifValue(physicalType, valueSource);
      case 22: return new PqdifValue(physicalType, valueSource);
      case 32: return new PqdifValue(physicalType, valueSource);
      case 40: return new PqdifValue(physicalType, valueSource);
      case 41: return new PqdifValue(physicalType, valueSource);
      case 42: return new PqdifValue(physicalType, { real: valueSource, image: 0.0 });
      case 43: return new PqdifValue(physicalType, { real: valueSource, image: 0.0 });
      case 50: {
        const day = Math.floor(valueSource);
        const sec = (valueSource - day) * 86400;
        return new PqdifValue(physicalType, { day, sec });
      }
      default:
        return new PqdifValue(physicalType, null);
    }
  }
}

module.exports = { PqdifValue };
