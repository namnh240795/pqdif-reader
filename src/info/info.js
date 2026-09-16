'use strict';

const { getTypeSize } = require('../constants/physicalTypes');
const { tagNameToGuid, tagGuidToName, idIntToName, idGuidToName, tagNameLowerToGuid, idNameLowerToGuid, idNameLowerToInt, physTypeToName } = require('../constants/nameLookup');

/**
 * CPQDIF_Info singleton equivalent - provides helper methods for
 * working with PQDIF constants and GUIDs.
 */
class PqdifInfo {
  /**
   * Get byte size of a physical type.
   * @param {number} physicalType - Physical type ID
   * @returns {number} Size in bytes
   */
  static getTypeSize(physicalType) {
    return getTypeSize(physicalType);
  }

  /**
   * Get human-readable tag name for a GUID Buffer.
   * @param {Buffer} tagGuid - 16-byte GUID Buffer
   * @returns {string|null} Tag name or null if not found
   */
  static getTagName(tagGuid) {
    const hex = tagGuid.toString('hex');
    return tagGuidToName.get(hex) || null;
  }

  /**
   * Get human-readable ID name for a GUID Buffer.
   * @param {Buffer} idGuid - 16-byte GUID Buffer
   * @returns {string|null} ID name or null if not found
   */
  static getIdName(idGuid) {
    const hex = idGuid.toString('hex');
    return idGuidToName.get(hex) || null;
  }

  /**
   * Get human-readable name for an integer ID.
   * @param {number} idInt - Integer ID value
   * @returns {string|null} ID name or null if not found
   */
  static getIdIntName(idInt) {
    return idIntToName.get(idInt) || null;
  }

  /**
   * Convert a 16-byte GUID Buffer to a lowercase hex string.
   * @param {Buffer} guid - 16-byte GUID Buffer
   * @returns {string} Hex string (32 chars)
   */
  static guidToHex(guid) {
    return guid.toString('hex');
  }

  /**
   * Compare two GUID Buffers for equality.
   * @param {Buffer} a - First 16-byte GUID Buffer
   * @param {Buffer} b - Second 16-byte GUID Buffer
   * @returns {boolean} True if equal
   */
  static guidEquals(a, b) {
    return a.equals(b);
  }

  /**
   * Parse a GUID string like "89738606-f1c3-11cf-9d89-0080c72e70a3" to a 16-byte Buffer.
   * The GUID string is in standard display format (big-endian), but the returned Buffer
   * is in wire format (little-endian for Data1/Data2/Data3) to match the constants.
   * @param {string} str - GUID string (with or without hyphens)
   * @returns {Buffer} 16-byte Buffer in wire format
   * @throws {Error} If the string is not a valid GUID
   */
  static guidFromString(str) {
    const hex = str.replace(/-/g, '');
    if (hex.length !== 32 || !/^[0-9a-fA-F]{32}$/.test(hex)) {
      throw new Error('Invalid GUID string: ' + str);
    }
    // GUID display format: Data1(8)-Data2(4)-Data3(4)-Data4(16)
    // Data1, Data2, Data3 are big-endian in display but little-endian in wire format
    const data1 = hex.substring(0, 8);
    const data2 = hex.substring(8, 12);
    const data3 = hex.substring(12, 16);
    const data4 = hex.substring(16, 32);
    // Reverse byte order for Data1 (4 bytes), Data2 (2 bytes), Data3 (2 bytes)
    const wireHex =
      data1.match(/../g).reverse().join('') +
      data2.match(/../g).reverse().join('') +
      data3.match(/../g).reverse().join('') +
      data4;
    return Buffer.from(wireHex, 'hex');
  }

  /**
   * Returns a short/alternate name for a tag GUID.
   * Since the JS port does not define alias macros, this returns the full name.
   * @param {Buffer} tagGuid - 16-byte GUID Buffer
   * @returns {string} Tag name or empty string if not found
   */
  static getAliasTag(tagGuid) {
    if (!tagGuid) return '';
    const hex = tagGuid.toString('hex');
    return tagGuidToName.get(hex) || '';
  }

  /**
   * Returns a short/alternate name for an ID GUID.
   * Since the JS port does not define alias macros, this returns the full name.
   * @param {Buffer} idGuid - 16-byte GUID Buffer
   * @returns {string} ID name or empty string if not found
   */
  static getAliasID(idGuid) {
    if (!idGuid) return '';
    const hex = idGuid.toString('hex');
    return idGuidToName.get(hex) || '';
  }

  /**
   * Returns a short/alternate name for an integer ID.
   * Since the JS port does not define alias macros, this returns the full name.
   * @param {number} idInt - Integer ID value
   * @returns {string} ID name or empty string if not found
   */
  static getAliasIDInt(idInt) {
    return idIntToName.get(idInt) || '';
  }

  /**
   * Reverse lookup: tag name -> tag GUID Buffer. Case-insensitive.
   * @param {string} name - Tag name (e.g., "tagContainer")
   * @returns {Buffer|null} 16-byte GUID Buffer or null if not found
   */
  static getTagFromName(name) {
    if (!name) return null;
    return tagNameLowerToGuid.get(name.toLowerCase()) || null;
  }

  /**
   * Reverse lookup: ID name -> ID GUID Buffer. Case-insensitive.
   * @param {string} name - ID name (e.g., "ID_QM_VOLTAGE")
   * @returns {Buffer|null} 16-byte GUID Buffer or null if not found
   */
  static getIDFromName(name) {
    if (!name) return null;
    return idNameLowerToGuid.get(name.toLowerCase()) || null;
  }

  /**
   * Reverse lookup: ID name -> integer ID. Case-insensitive.
   * @param {string} name - ID name (e.g., "ID_QM_VOLTAGE")
   * @returns {number|null} Integer ID or null if not found
   */
  static getIDFromNameInt(name) {
    if (!name) return null;
    const result = idNameLowerToInt.get(name.toLowerCase());
    return result !== undefined ? result : null;
  }

  /**
   * Returns the string name of a physical type.
   * @param {number} physicalType - Physical type ID (e.g., 40 for REAL4)
   * @returns {string} Name string or empty string if not found
   */
  static getNamePhysType(physicalType) {
    return physTypeToName.get(physicalType) || '';
  }
}

/**
 * Compare two GUID Buffers for equality (standalone export).
 * @param {Buffer} a - First 16-byte GUID Buffer
 * @param {Buffer} b - Second 16-byte GUID Buffer
 * @returns {boolean} True if equal
 */
function guidEquals(a, b) {
  if (!a || !b) return false;
  if (a.length !== b.length) return false;
  return a.equals(b);
}

module.exports = { PqdifInfo, guidEquals };
