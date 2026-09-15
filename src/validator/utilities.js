'use strict';

const { PqdifInfo } = require('../info/info');

/**
 * Utility classes ported from C# Conversion.cs, Information.cs, and XML.cs
 */

// ---------------------------------------------------------------------------
// Conversion (from Conversion.cs)
// ---------------------------------------------------------------------------

/**
 * Converts a null or empty value to a default.
 * @param {*} value
 * @param {*} nullValue
 * @returns {*}
 */
function fixNull(value, nullValue) {
  if (value === null || value === undefined) return nullValue;
  if (typeof value === 'string' && value.length === 0) return nullValue;
  return value;
}

/**
 * Returns the integer portion of a number.
 * @param {*} number
 * @returns {number}
 */
function intPortion(number) {
  if (!isNumeric(number)) return 0;
  const parsed = parseInt(String(number), 10);
  return isNaN(parsed) ? 0 : parsed;
}

// ---------------------------------------------------------------------------
// Information (from Information.cs)
// ---------------------------------------------------------------------------

/**
 * Returns true if the expression is a valid Date or can be parsed as one.
 * @param {*} expression
 * @returns {boolean}
 */
function isDate(expression) {
  if (expression === null || expression === undefined) return false;
  if (expression instanceof Date) return true;
  if (typeof expression === 'string') return !isNaN(Date.parse(expression));
  if (expression && typeof expression === 'object' && expression.day !== undefined) return true; // PQDIF timestamp
  return false;
}

/**
 * Returns true if the expression can be evaluated as a number.
 * @param {*} expression
 * @returns {boolean}
 */
function isNumeric(expression) {
  if (expression === null || expression === undefined) return false;
  if (typeof expression === 'number') return true;
  if (typeof expression === 'boolean') return true;
  if (typeof expression === 'string') return !isNaN(Number(expression)) && expression.trim().length > 0;
  return false;
}

// ---------------------------------------------------------------------------
// XML (from XML.cs)
// ---------------------------------------------------------------------------

/**
 * Formats a floating point number with maximum precision.
 * @param {number} value
 * @returns {string}
 */
function formatFloat(value) {
  let s = value.toPrecision(32);
  // Trim trailing zeros
  s = s.replace(/0+$/, '');
  // Remove trailing dot if present
  if (s.endsWith('.')) s = s.slice(0, -1);
  return s;
}

/**
 * HTML-encode a string for safe XML output.
 * @param {string} text
 * @returns {string}
 */
function htmlEncode(text) {
  return text
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;');
}

/**
 * Get the XML element string for a GUID tag + GUID value.
 * @param {Buffer} tag
 * @param {Buffer} value
 * @returns {string}
 */
function xmlGetElementGuidValue(tag, value) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  const idName = PqdifInfo.getIdName(value) || (value ? value.toString('hex') : '');
  return `<${tagName} Type="GUID">${idName}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + string value.
 * @param {Buffer} tag
 * @param {string} value
 * @returns {string}
 */
function xmlGetElementStringValue(tag, value) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  return `<${tagName} Type="CHAR1" Size="${value.length}">${htmlEncode(value)}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + double value.
 * @param {Buffer} tag
 * @param {number} value
 * @returns {string}
 */
function xmlGetElementDoubleValue(tag, value) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  return `<${tagName} Type="REAL8">${formatFloat(value)}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + uint value.
 * @param {Buffer} tag
 * @param {number} value
 * @returns {string}
 */
function xmlGetElementUintValue(tag, value) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  const idName = PqdifInfo.getIdIntName(value) || String(value);
  return `<${tagName} Type="UINT">${idName}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + boolean value.
 * @param {Buffer} tag
 * @param {boolean} value
 * @returns {string}
 */
function xmlGetElementBoolValue(tag, value) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  return `<${tagName} Type="BOOL4">${value ? 1 : 0}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + date value.
 * @param {Buffer} tag
 * @param {Date} value
 * @returns {string}
 */
function xmlGetElementDateValue(tag, value) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  const dateStr = value.toISOString().replace('T', ' ').replace('Z', '').substring(0, 23);
  return `<${tagName} Type="DT">${dateStr}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + array value.
 * @param {Buffer} tag
 * @param {Array} value
 * @param {number|null} maxSeriesValues
 * @returns {string}
 */
function xmlGetElementArrayValue(tag, value, maxSeriesValues) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');
  if (!value || value.length === 0) {
    return `<${tagName}/>`;
  }

  let rows = value.length;
  let ellipsis = '';
  if (maxSeriesValues !== null && maxSeriesValues !== undefined && rows > maxSeriesValues) {
    rows = maxSeriesValues;
    ellipsis = '...';
  }

  let elementType = '';
  let items = [];

  if (value instanceof Uint16Array || (Array.isArray(value) && value.length > 0 && typeof value[0] === 'number' && value[0] === (value[0] & 0xFFFF) && value[0] >= 0)) {
    // Check if values look like uint16
    elementType = ' Type="UINT2" ';
    for (let i = 0; i < rows; i++) items.push(String(value[i]));
  } else if (value instanceof Int16Array) {
    elementType = ' Type="INT2" ';
    for (let i = 0; i < rows; i++) items.push(String(value[i]));
  } else if (value instanceof Uint32Array || (Array.isArray(value) && value.length > 0 && Number.isInteger(value[0]) && value[0] >= 0 && value[0] <= 0xFFFFFFFF)) {
    elementType = ' Type="UINT" ';
    for (let i = 0; i < rows; i++) items.push(String(value[i]));
  } else if (value instanceof Int32Array || (Array.isArray(value) && value.length > 0 && Number.isInteger(value[0]))) {
    elementType = ' Type="INT4" ';
    for (let i = 0; i < rows; i++) items.push(String(value[i]));
  } else if (value instanceof Float32Array) {
    elementType = ' Type="REAL4" ';
    for (let i = 0; i < rows; i++) items.push(formatFloat(value[i]));
  } else if (value instanceof Float64Array || (Array.isArray(value) && value.length > 0 && typeof value[0] === 'number')) {
    elementType = ' Type="REAL8" ';
    for (let i = 0; i < rows; i++) items.push(formatFloat(value[i]));
  } else if (value instanceof Array && value.length > 0 && typeof value[0] === 'boolean') {
    elementType = ' Type="BOOL4" ';
    for (let i = 0; i < rows; i++) items.push(value[i] ? '1' : '0');
  } else {
    // Generic: try to determine type from Buffer
    if (Buffer.isBuffer(value)) {
      elementType = ' Type="UINT" ';
      for (let i = 0; i < rows; i++) items.push(String(value.readUInt32LE(i * 4)));
    } else {
      elementType = ' Type="REAL8" ';
      for (let i = 0; i < rows; i++) items.push(formatFloat(Number(value[i])));
    }
  }

  return `<${tagName}${elementType} Size="${rows}">${items.join(',')}${ellipsis}</${tagName}>`;
}

/**
 * Get the XML element string for a GUID tag + object value (dispatches by type).
 * @param {Buffer} tag
 * @param {*} value
 * @param {number|null} [maxSeriesValues=null]
 * @returns {string}
 */
function xmlGetElement(tag, value, maxSeriesValues) {
  const tagName = PqdifInfo.getTagName(tag) || tag.toString('hex');

  if (value === null || value === undefined) {
    return `<${tagName}/>`;
  }

  // Buffer (16 bytes = GUID)
  if (Buffer.isBuffer(value) && value.length === 16) {
    return xmlGetElementGuidValue(tag, value);
  }

  if (typeof value === 'string') {
    return xmlGetElementStringValue(tag, value);
  }

  if (typeof value === 'number') {
    return xmlGetElementDoubleValue(tag, value);
  }

  if (typeof value === 'boolean') {
    return xmlGetElementBoolValue(tag, value);
  }

  if (value instanceof Date) {
    return xmlGetElementDateValue(tag, value);
  }

  if (Array.isArray(value) || ArrayBuffer.isView(value)) {
    return xmlGetElementArrayValue(tag, value, maxSeriesValues !== undefined ? maxSeriesValues : null);
  }

  // PQDIF timestamp { day, sec }
  if (value && typeof value === 'object' && value.day !== undefined) {
    const date = pqdifTimestampToDate(value);
    return xmlGetElementDateValue(tag, date);
  }

  return `<${tagName}/>`;
}

// ---------------------------------------------------------------------------
// PQDIF Timestamp conversions
// ---------------------------------------------------------------------------

// PQDIF epoch: January 1, 1900
// JS epoch:   January 1, 1970
// Difference: 70 years in days = 25569
// OADate epoch: December 30, 1899
// So PQDIF day 1 = OADate 1, PQDIF day N = OADate N
// And OADate = (date.getTime() / 86400000) + 25569
const PQDIF_OADATE_OFFSET = 25569;

/**
 * Convert a PQDIF timestamp { day, sec } to a JavaScript Date.
 * @param {Object} ts - { day: number, sec: number }
 * @returns {Date}
 */
function pqdifTimestampToDate(ts) {
  if (!ts) return new Date(0);
  const oaDate = ts.day + ts.sec / 86400;
  const ms = (oaDate - PQDIF_OADATE_OFFSET) * 86400000;
  return new Date(ms);
}

/**
 * Convert a JavaScript Date to a PQDIF timestamp { day, sec }.
 * @param {Date} date
 * @returns {Object} { day: number, sec: number }
 */
function dateToPqdifTimestamp(date) {
  if (!date) return { day: 0, sec: 0 };
  const ms = date.getTime();
  const oaDate = (ms / 86400000) + PQDIF_OADATE_OFFSET;
  const day = Math.floor(oaDate);
  const sec = (oaDate - day) * 86400;
  return { day, sec };
}

module.exports = {
  fixNull,
  intPortion,
  isDate,
  isNumeric,
  formatFloat,
  htmlEncode,
  xmlGetElement: xmlGetElement,
  xmlGetElementGuidValue,
  xmlGetElementStringValue,
  xmlGetElementDoubleValue,
  xmlGetElementUintValue,
  xmlGetElementBoolValue,
  xmlGetElementDateValue,
  xmlGetElementArrayValue,
  pqdifTimestampToDate,
  dateToPqdifTimestamp,
};
