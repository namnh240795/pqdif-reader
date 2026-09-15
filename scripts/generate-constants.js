#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');

const BASE = path.resolve(__dirname, '..');
const LG_H = path.join(BASE, 'pqdiflib', 'pqdif_lg.h');
const ID_H = path.join(BASE, 'pqdiflib', 'pqdif_id.h');
const OUT = path.join(BASE, 'src', 'constants');

// Parse a C++ GUID definition:
// const GUID name = { 0xXXXXXXXX, 0xXXXX, 0xXXXX, { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX } };
// Returns { name, data1, data2, data3, data4[] }
function parseGuidDef(line) {
  // Match both "const GUID name = { ... }" and "PQDIF_DEFINE_GUID(name, ...)"
  const guidRegex = /(?:const\s+GUID\s+|PQDIF_DEFINE_GUID\s*\(\s*)(\w+)\s*[,\s]*\{\s*(0[xX][0-9a-fA-F]+)\s*,\s*(0[xX][0-9a-fA-F]+)\s*,\s*(0[xX][0-9a-fA-F]+)\s*,\s*\{\s*([\s\S]*?)\}\s*\}/i;
  const m = line.match(guidRegex);
  if (!m) return null;

  const name = m[1];
  const data1 = parseInt(m[2], 16);
  const data2 = parseInt(m[3], 16);
  const data3 = parseInt(m[4], 16);
  const data4Str = m[5];
  const data4Parts = data4Str.match(/0[xX][0-9a-fA-F]+/gi) || [];
  const data4 = data4Parts.map(h => parseInt(h, 16));

  return { name, data1, data2, data3, data4 };
}

// Convert parsed GUID to JS Buffer hex array
function guidToBufferArray(data1, data2, data3, data4) {
  // Data1 (4 bytes LE)
  const b0 = (data1) & 0xFF;
  const b1 = (data1 >> 8) & 0xFF;
  const b2 = (data1 >> 16) & 0xFF;
  const b3 = (data1 >> 24) & 0xFF;
  // Data2 (2 bytes LE)
  const b4 = (data2) & 0xFF;
  const b5 = (data2 >> 8) & 0xFF;
  // Data3 (2 bytes LE)
  const b6 = (data3) & 0xFF;
  const b7 = (data3 >> 8) & 0xFF;
  // Data4 (8 bytes, as-is)
  const bytes = [b0, b1, b2, b3, b4, b5, b6, b7, ...data4.slice(0, 8)];
  return bytes;
}

function guidToHexString(data1, data2, data3, data4) {
  const d4 = data4.slice(0, 8).map(b => b.toString(16).padStart(2, '0'));
  const s1 = data1.toString(16).padStart(8, '0');
  const s2 = data2.toString(16).padStart(4, '0');
  const s3 = data3.toString(16).padStart(4, '0');
  return `${s1}-${s2}-${s3}-${d4.slice(0, 2).join('')}-${d4.slice(2).join('')}`;
}

// ===== Parse pqdif_lg.h for tag GUIDs =====
const lgContentRaw = fs.readFileSync(LG_H, 'utf8');
const lgContent = stripCppComments(lgContentRaw);
const tagGuids = [];

// Helper to skip C++ comments in GUID definitions
function stripCppComments(str) {
  return str.replace(/\/\/[^\n]*/g, '').replace(/\/\*[\s\S]*?\*\//g, '');
}

// Parse all const GUID definitions
// We first strip comments from the relevant section, then parse
const guidRegex = /const\s+GUID\s+(\w+)\s*=\s*\{\s*(0[xX][0-9a-fA-F]+)\s*,\s*(0[xX][0-9a-fA-F]+)\s*,\s*(0[xX][0-9a-fA-F]+)\s*,\s*\{\s*([\s\S]*?)\}\s*\}/gi;
let m;
while ((m = guidRegex.exec(lgContent)) !== null) {
  const name = m[1];
  const data1 = parseInt(m[2], 16);
  const data2 = parseInt(m[3], 16);
  const data3 = parseInt(m[4], 16);
  const data4Parts = m[5].match(/0[xX][0-9a-fA-F]+/gi) || [];
  const data4 = data4Parts.map(h => parseInt(h, 16));
  tagGuids.push({ name, data1, data2, data3, data4 });
}

// ===== Parse pqdif_id.h for all constants =====
const idContentRaw = fs.readFileSync(ID_H, 'utf8');
const idContent = stripCppComments(idContentRaw);

// Parse GUID-type IDs
const idGuids = [];
const idGuidRegex = /const\s+GUID\s+(\w+)\s*=\s*\{\s*(0[xX][0-9a-fA-F]+)\s*,\s*(0[xX][0-9a-fA-F]+)\s*,\s*(0[xX][0-9a-fA-F]+)\s*,\s*\{\s*([\s\S]*?)\}\s*\}/gi;
while ((m = idGuidRegex.exec(idContent)) !== null) {
  const name = m[1];
  const data1 = parseInt(m[2], 16);
  const data2 = parseInt(m[3], 16);
  const data3 = parseInt(m[4], 16);
  const data4Parts = m[5].match(/0[xX][0-9a-fA-F]+/gi) || [];
  const data4 = data4Parts.map(h => parseInt(h, 16));
  idGuids.push({ name, data1, data2, data3, data4 });
}

// Parse integer-type IDs
const idInts = [];
const intRegex = /const\s+int\s+(\w+)\s*=\s*(0[xX][0-9a-fA-F]+|\d+)\s*;/gi;
while ((m = intRegex.exec(idContent)) !== null) {
  const name = m[1];
  const rawVal = m[2];
  const value = rawVal.startsWith('0x') || rawVal.startsWith('0X') ? parseInt(rawVal, 16) : parseInt(rawVal, 10);
  idInts.push({ name, value });
}

// ===== Write physicalTypes.js =====
const physTypesContent = `'use strict';

// Physical type IDs from pqdif_ph.h
exports.ID_PHYS_TYPE_BOOLEAN1 = 1;
exports.ID_PHYS_TYPE_BOOLEAN2 = 2;
exports.ID_PHYS_TYPE_BOOLEAN4 = 3;
exports.ID_PHYS_TYPE_CHAR1 = 10;
exports.ID_PHYS_TYPE_CHAR2 = 11;
exports.ID_PHYS_TYPE_INTEGER1 = 20;
exports.ID_PHYS_TYPE_INTEGER2 = 21;
exports.ID_PHYS_TYPE_INTEGER4 = 22;
exports.ID_PHYS_TYPE_UNS_INTEGER1 = 30;
exports.ID_PHYS_TYPE_UNS_INTEGER2 = 31;
exports.ID_PHYS_TYPE_UNS_INTEGER4 = 32;
exports.ID_PHYS_TYPE_REAL4 = 40;
exports.ID_PHYS_TYPE_REAL8 = 41;
exports.ID_PHYS_TYPE_COMPLEX8 = 42;
exports.ID_PHYS_TYPE_COMPLEX16 = 43;
exports.ID_PHYS_TYPE_TIMESTAMPPQDIF = 50;
exports.ID_PHYS_TYPE_GUID = 60;

// Helper constants
exports.EXCEL_DAYCOUNT_ADJUST = 25569;
exports.SECONDS_PER_DAY = 86400;
exports.RECORD_HEADER_SIZE = 64;

/**
 * Returns the byte size of a physical type.
 * @param {number} physicalType
 * @returns {number}
 */
function getTypeSize(physicalType) {
  switch (physicalType) {
    case 1:  // BOOLEAN1
    case 10: // CHAR1
    case 20: // INTEGER1
    case 30: // UNS_INTEGER1
      return 1;
    case 2:  // BOOLEAN2
    case 11: // CHAR2
    case 21: // INTEGER2
    case 31: // UNS_INTEGER2
      return 2;
    case 3:  // BOOLEAN4
    case 22: // INTEGER4
    case 32: // UNS_INTEGER4
    case 40: // REAL4
      return 4;
    case 41: // REAL8
    case 42: // COMPLEX8
      return 8;
    case 50: // TIMESTAMPPQDIF (UINT4 day + REAL8 sec)
      return 12;
    case 43: // COMPLEX16
    case 60: // GUID
      return 16;
    default:
      return 0;
  }
}

exports.getTypeSize = getTypeSize;
`;
fs.writeFileSync(path.join(OUT, 'physicalTypes.js'), physTypesContent);

// ===== Write elementTypes.js =====
const elemTypesContent = `'use strict';

exports.ID_ELEMENT_TYPE_COLLECTION = 1;
exports.ID_ELEMENT_TYPE_SCALAR = 2;
exports.ID_ELEMENT_TYPE_VECTOR = 3;
`;
fs.writeFileSync(path.join(OUT, 'elementTypes.js'), elemTypesContent);

// ===== Write compressionIds.js =====
const compIdsContent = `'use strict';

exports.ID_COMP_STYLE_NONE = 0;
exports.ID_COMP_STYLE_TOTALFILE = 1; // deprecated
exports.ID_COMP_STYLE_RECORDLEVEL = 2;
exports.ID_COMP_ALG_NONE = 0;
exports.ID_COMP_ALG_ZLIB = 1;
exports.ID_COMP_ALG_PKZIPCL = 64; // deprecated
`;
fs.writeFileSync(path.join(OUT, 'compressionIds.js'), compIdsContent);

// ===== Write tagGuids.js =====
let tagGuidsContent = `'use strict';

// Tag GUIDs from pqdif_lg.h
`;
for (const g of tagGuids) {
  const bytes = guidToBufferArray(g.data1, g.data2, g.data3, g.data4);
  tagGuidsContent += `exports.${g.name} = Buffer.from([${bytes.map(b => '0x' + b.toString(16).padStart(2, '0')).join(', ')}]);\n`;
}
fs.writeFileSync(path.join(OUT, 'tagGuids.js'), tagGuidsContent);

// ===== Write idGuids.js =====
let idGuidsContent = `'use strict';

// GUID-type IDs from pqdif_id.h
`;
for (const g of idGuids) {
  const bytes = guidToBufferArray(g.data1, g.data2, g.data3, g.data4);
  idGuidsContent += `exports.${g.name} = Buffer.from([${bytes.map(b => '0x' + b.toString(16).padStart(2, '0')).join(', ')}]);\n`;
}
fs.writeFileSync(path.join(OUT, 'idGuids.js'), idGuidsContent);

// ===== Write idIntegers.js =====
let idIntsContent = `'use strict';

// Integer-type IDs from pqdif_id.h
`;
for (const i of idInts) {
  idIntsContent += `exports.${i.name} = ${i.value};\n`;
}
fs.writeFileSync(path.join(OUT, 'idIntegers.js'), idIntsContent);

// ===== Write index.js =====
const indexContent = `'use strict';

const physicalTypes = require('./physicalTypes');
const elementTypes = require('./elementTypes');
const compressionIds = require('./compressionIds');
const tagGuids = require('./tagGuids');
const idGuids = require('./idGuids');
const idIntegers = require('./idIntegers');

module.exports = {
  ...physicalTypes,
  ...elementTypes,
  ...compressionIds,
  ...tagGuids,
  ...idGuids,
  ...idIntegers,
};
`;
fs.writeFileSync(path.join(OUT, 'index.js'), indexContent);

// ===== Write nameLookup.js =====
// Build tagNameToGuid and tagGuidToName
let nameLookupContent = `'use strict';

// Lookup maps for tags and IDs
const tagGuids = require('./tagGuids');
const idGuids = require('./idGuids');
const idIntegers = require('./idIntegers');

// Map<string, Buffer> - tag name -> GUID Buffer
const tagNameToGuid = new Map();
// Map<string, string> - hex GUID string -> tag name
const tagGuidToName = new Map();
// Map<number, string> - integer ID -> name
const idIntToName = new Map();
// Map<string, string> - hex GUID string -> ID name
const idGuidToName = new Map();

// Populate tag maps
`;

for (const g of tagGuids) {
  nameLookupContent += `tagNameToGuid.set('${g.name}', tagGuids.${g.name});\n`;
}

nameLookupContent += `\n// Populate tag GUID to name (hex key)\n`;
for (const g of tagGuids) {
  const bytes = guidToBufferArray(g.data1, g.data2, g.data3, g.data4);
  const hex = Buffer.from(bytes).toString('hex');
  nameLookupContent += `tagGuidToName.set('${hex}', '${g.name}');\n`;
}

nameLookupContent += `\n// Populate integer ID to name\n`;
for (const i of idInts) {
  nameLookupContent += `idIntToName.set(${i.value}, '${i.name}');\n`;
}

nameLookupContent += `\n// Populate ID GUID to name (hex key)\n`;
for (const g of idGuids) {
  const bytes = guidToBufferArray(g.data1, g.data2, g.data3, g.data4);
  const hex = Buffer.from(bytes).toString('hex');
  nameLookupContent += `idGuidToName.set('${hex}', '${g.name}');\n`;
}

nameLookupContent += `
module.exports = { tagNameToGuid, tagGuidToName, idIntToName, idGuidToName };
`;
fs.writeFileSync(path.join(OUT, 'nameLookup.js'), nameLookupContent);

// ===== Write info/info.js =====
const infoDir = path.join(BASE, 'src', 'info');
const infoContent = `'use strict';

const { getTypeSize } = require('../constants/physicalTypes');
const { tagNameToGuid, tagGuidToName, idIntToName, idGuidToName } = require('../constants/nameLookup');

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
   * @param {string} str - GUID string (with or without hyphens)
   * @returns {Buffer} 16-byte Buffer
   * @throws {Error} If the string is not a valid GUID
   */
  static guidFromString(str) {
    // Remove hyphens and convert hex pairs to bytes
    const hex = str.replace(/-/g, '');
    if (hex.length !== 32 || !/^[0-9a-fA-F]{32}$/.test(hex)) {
      throw new Error('Invalid GUID string: ' + str);
    }
    return Buffer.from(hex, 'hex');
  }
}

module.exports = { PqdifInfo };
`;
fs.writeFileSync(path.join(infoDir, 'info.js'), infoContent);

// ===== Write info/index.js =====
const infoIndexContent = `'use strict';

const { PqdifInfo } = require('./info');
module.exports = { PqdifInfo };
`;
fs.writeFileSync(path.join(infoDir, 'index.js'), infoIndexContent);

// Report counts
console.log(`Generated files in ${OUT}`);
console.log(`  tagGuids: ${tagGuids.length} GUIDs`);
console.log(`  idGuids: ${idGuids.length} GUIDs`);
console.log(`  idIntegers: ${idInts.length} integers`);
console.log(`Generated info/info.js and info/index.js`);
