'use strict';

/**
 * Record type enumeration.
 */
const RecType = {
  Blank: 0,
  Container: 1,
  RecDataSource: 2,
  RecMonitorSettings: 3,
  RecObservation: 4,
};

/**
 * Holds a reference to a parsed PQDIF record along with its type and cross-references.
 */
class RecordHolder {
  /**
   * @param {number} recIndex - Index of the record in the file
   * @param {number} recType - One of the RecType values
   * @param {Object} record - The parsed record wrapper (Container, RecDataSource, etc.)
   */
  constructor(recIndex, recType, record) {
    /** @type {number} Index of the record in the file */
    this.recIndex = recIndex;

    /** @type {number} Record type (see RecType) */
    this.recType = recType;

    /** @type {Object} The parsed record wrapper */
    this.record = record;

    /** @type {Object|null} Cross-reference to the most recent RecDataSource */
    this.dataSource = null;

    /** @type {Object|null} Cross-reference to the most recent RecMonitorSettings */
    this.monitorSettings = null;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      recIndex: this.recIndex,
      recType: this.recType,
      record: this.record ? this.record.getJSON() : null
    };
  }
}

module.exports = { RecordHolder, RecType };
