'use strict';

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

// Record signature GUID (4a111440-e49f-11cf-9900-505144494600) in mixed-endian bytes
exports.GUID_RECORD_SIGNATURE_PQDIF = Buffer.from([
  0x40, 0x14, 0x11, 0x4a,
  0x9f, 0xe4,
  0xcf, 0x11,
  0x99, 0x00,
  0x50, 0x51, 0x44, 0x49, 0x46, 0x00
]);

// Storage method flags
exports.ID_SERIES_METHOD_VALUES = 1;
exports.ID_SERIES_METHOD_INCREMENT = 2;
exports.ID_SERIES_METHOD_SCALED = 4;

// Trigger method IDs
exports.ID_TRIGGER_METH_NONE = 0;
exports.ID_TRIGGER_METH_CHANNEL = 1;

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
      return 1; // Match C++ behavior: unknown types default to 1 byte
  }
}

exports.getTypeSize = getTypeSize;
