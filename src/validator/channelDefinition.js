'use strict';

const { PqdifInfo, guidEquals } = require('../info/info');
const { Logger } = require('../logger');
const { LogLevels } = require('../logger');
const tagGuids = require('../constants/tagGuids');
const { xmlGetElement } = require('./utilities');
const SeriesDefinition = require('./seriesDefinition');
const idGuids = require('../constants/idGuids');

/**
 * Channel definition - ported from OneChannelDefn.cs.
 */
class ChannelDefinition {
  constructor() {
    /** @type {string} Channel name */
    this.channelName = '';

    /** @type {number} Phase ID */
    this.phaseID = 0;

    /** @type {string} Other channel identifier */
    this.otherChannelIdentifier = '';

    /** @type {string} Group name */
    this.groupName = '';

    /** @type {Buffer} Quantity type GUID */
    this.quantityTypeID = null;

    /** @type {number} Quantity measured ID */
    this.quantityMeasuredID = 0;

    /** @type {number} Physical channel */
    this.physicalChannel = 0;

    /** @type {string} Quantity name */
    this.quantityName = '';

    /** @type {number} Primary series index */
    this.primarySeriesIdx = 0;

    /** @type {SeriesDefinition[]} Series definitions */
    this.seriesDefns = [];

    /** @type {Logger} */
    this.loggerApplication = new Logger();

    /** @type {Logger} */
    this.loggerCompliance = new Logger();
  }

  /**
   * Create from a Collection element.
   * @param {import('../core/collection').Collection} coll
   * @returns {ChannelDefinition}
   */
  static fromCollection(coll) {
    const defn = new ChannelDefinition();
    if (!coll) return defn;

    const count = coll.getCount ? coll.getCount() : 0;
    for (let i = 0; i < count; i++) {
      const el = coll.getElement(i);
      if (!el || !el.getTag()) continue;
      const tag = el.getTag();

      try {
        if (guidEquals(tag, tagGuids.tagChannelName)) {
          defn.channelName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhaseID)) {
          defn.phaseID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagOtherChannelIdentifier)) {
          defn.otherChannelIdentifier = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagGroupName)) {
          defn.groupName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagQuantityTypeID)) {
          const r = el.getValueGUID ? el.getValueGUID() : null;
          if (r && r.status) defn.quantityTypeID = r.value;
        } else if (guidEquals(tag, tagGuids.tagQuantityMeasuredID)) {
          defn.quantityMeasuredID = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPhysicalChannel)) {
          defn.physicalChannel = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagQuantityName)) {
          defn.quantityName = _getStringFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagPrimarySeriesIdx)) {
          defn.primarySeriesIdx = _getUintFromElement(el);
        } else if (guidEquals(tag, tagGuids.tagSeriesDefns)) {
          // Series definitions sub-collection
          if (typeof el.getCount === 'function') {
            const seriesCount = el.getCount();
            if (seriesCount === 0) {
              defn.loggerCompliance.log('tagSeriesDefns must have at least one tagOneSeriesDefn record.', LogLevels.Error);
            }
            for (let j = 0; j < seriesCount; j++) {
              const seriesEl = el.getElement(j);
              if (seriesEl && typeof seriesEl.getCount === 'function') {
                const sd = SeriesDefinition.fromCollection(seriesEl);
                defn.seriesDefns.push(sd);
              }
            }
          }
        } else if (guidEquals(tag, tagGuids.tagBlank)) {
          // ignore
        } else {
          defn.loggerCompliance.log('Unknown or Unexpected Tag: ' + (PqdifInfo.getTagName(tag) || tag.toString('hex')), LogLevels.Info);
        }
      } catch (ex) {
        defn.loggerApplication.log('Error reading tag: ' + ex.message, LogLevels.Error);
      }
    }

    // Check required tags
    if (count === 0) defn.loggerCompliance.log('tagChannelDefns Must have at least tagOneChannelDefn Record.', LogLevels.Error);
    if (defn.phaseID === 0) defn.loggerCompliance.log('Required tagPhaseID is Missing.', LogLevels.Error);
    if (!defn.quantityTypeID) defn.loggerCompliance.log('Required tagQuantityTypeID is Missing.', LogLevels.Error);
    if (defn.quantityMeasuredID === 0) defn.loggerCompliance.log('Required tagQuantityMeasuredID is Missing.', LogLevels.Error);
    if (defn.seriesDefns.length === 0) defn.loggerCompliance.log('Required tagSeriesDefns is Missing.', LogLevels.Error);

    // Validate series definitions for quantity type
    ChannelDefinition.validateSeriesDefinitionsForQuantityType(defn);

    return defn;
  }

  /**
   * Validate series definitions for the given quantity type.
   * Ported from OneChannelDefn.cs lines 309-390.
   * @param {ChannelDefinition} defn
   */
  static validateSeriesDefinitionsForQuantityType(defn) {
    if (!defn.quantityTypeID || defn.seriesDefns.length === 0) return;

    const vt = (idx) => {
      if (idx < defn.seriesDefns.length && defn.seriesDefns[idx].valueTypeID) {
        return defn.seriesDefns[idx].valueTypeID;
      }
      return null;
    };
    const eq = (a, b) => a !== null && b !== null && guidEquals(a, b);

    const idTime = idGuids.ID_SERIES_VALUE_TYPE_TIME;
    const idVal = idGuids.ID_SERIES_VALUE_TYPE_VAL;
    const idFreq = idGuids.ID_SERIES_VALUE_TYPE_FREQUENCY;
    const idBinId = idGuids.ID_SERIES_VALUE_TYPE_BINID;
    const idBinHigh = idGuids.ID_SERIES_VALUE_TYPE_BINHIGH;
    const idBinLow = idGuids.ID_SERIES_VALUE_TYPE_BINLOW;
    const idCount = idGuids.ID_SERIES_VALUE_TYPE_COUNT;
    const idStatus = idGuids.ID_SERIES_VALUE_TYPE_STATUS;
    const idMin = idGuids.ID_SERIES_VALUE_TYPE_MIN;
    const idMax = idGuids.ID_SERIES_VALUE_TYPE_MAX;
    const idDuration = idGuids.ID_SERIES_VALUE_TYPE_DURATION;
    const idInst = idGuids.ID_SERIES_VALUE_TYPE_INST;
    const idPhaseAngle = idGuids.ID_SERIES_VALUE_TYPE_PHASEANGLE;
    const idProb = idGuids.ID_SERIES_VALUE_TYPE_PROB;

    const count = defn.seriesDefns.length;
    const qt = defn.quantityTypeID;

    if (guidEquals(qt, idGuids.ID_QT_WAVEFORM)) {
      // WAVEFORM: expects 2 series (TIME + VAL)
      if (count !== 2) {
        defn.loggerCompliance.log('WAVEFORM channel should have 2 series definitions (TIME + VAL), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idTime)) defn.loggerCompliance.log('WAVEFORM series[0] should be TIME.', LogLevels.Warning);
        if (!eq(vt(1), idVal)) defn.loggerCompliance.log('WAVEFORM series[1] should be VAL.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_VALUELOG)) {
      // VALUELOG: expects 2 series (TIME + VAL)
      if (count !== 2) {
        defn.loggerCompliance.log('VALUELOG channel should have 2 series definitions (TIME + VAL), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idTime)) defn.loggerCompliance.log('VALUELOG series[0] should be TIME.', LogLevels.Warning);
        if (!eq(vt(1), idVal)) defn.loggerCompliance.log('VALUELOG series[1] should be VAL.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_PHASOR)) {
      // PHASOR: expects 2 series (TIME + VAL)
      if (count !== 2) {
        defn.loggerCompliance.log('PHASOR channel should have 2 series definitions (TIME + VAL), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idTime)) defn.loggerCompliance.log('PHASOR series[0] should be TIME.', LogLevels.Warning);
        if (!eq(vt(1), idVal)) defn.loggerCompliance.log('PHASOR series[1] should be VAL.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_RESPONSE)) {
      // RESPONSE: expects 2 series (FREQ + VAL)
      if (count !== 2) {
        defn.loggerCompliance.log('RESPONSE channel should have 2 series definitions (FREQ + VAL), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idFreq)) defn.loggerCompliance.log('RESPONSE series[0] should be FREQUENCY.', LogLevels.Warning);
        if (!eq(vt(1), idVal)) defn.loggerCompliance.log('RESPONSE series[1] should be VAL.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_FLASH)) {
      // FLASH: expects 1 series (VAL)
      if (count !== 1) {
        defn.loggerCompliance.log('FLASH channel should have 1 series definition (VAL), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idVal)) defn.loggerCompliance.log('FLASH series[0] should be VAL.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_HISTOGRAM)) {
      // HISTOGRAM: expects 3 series (BINID + BINHIGH + COUNT)
      if (count !== 3) {
        defn.loggerCompliance.log('HISTOGRAM channel should have 3 series definitions (BINID + BINHIGH + COUNT), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idBinId)) defn.loggerCompliance.log('HISTOGRAM series[0] should be BINID.', LogLevels.Warning);
        if (!eq(vt(1), idBinHigh)) defn.loggerCompliance.log('HISTOGRAM series[1] should be BINHIGH.', LogLevels.Warning);
        if (!eq(vt(2), idCount)) defn.loggerCompliance.log('HISTOGRAM series[2] should be COUNT.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_HISTOGRAM3D)) {
      // HISTOGRAM3D: expects 4 series (BINID + BINHIGH + BINLOW + COUNT)
      if (count !== 4) {
        defn.loggerCompliance.log('HISTOGRAM3D channel should have 4 series definitions (BINID + BINHIGH + BINLOW + COUNT), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idBinId)) defn.loggerCompliance.log('HISTOGRAM3D series[0] should be BINID.', LogLevels.Warning);
        if (!eq(vt(1), idBinHigh)) defn.loggerCompliance.log('HISTOGRAM3D series[1] should be BINHIGH.', LogLevels.Warning);
        if (!eq(vt(2), idBinLow)) defn.loggerCompliance.log('HISTOGRAM3D series[2] should be BINLOW.', LogLevels.Warning);
        if (!eq(vt(3), idCount)) defn.loggerCompliance.log('HISTOGRAM3D series[3] should be COUNT.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_CPF)) {
      // CPF: expects 2 series (MAG + ANGLE) - using INST and PHASEANGLE as proxies
      if (count !== 2) {
        defn.loggerCompliance.log('CPF channel should have 2 series definitions (MAG + ANGLE), found ' + count + '.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_XY)) {
      // XY: expects 2 series (X + Y) - using VAL for both as there's no separate X/Y type
      if (count !== 2) {
        defn.loggerCompliance.log('XY channel should have 2 series definitions (X + Y), found ' + count + '.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_XYZ)) {
      // XYZ: expects 3 series
      if (count !== 3) {
        defn.loggerCompliance.log('XYZ channel should have 3 series definitions, found ' + count + '.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_MAGDUR)) {
      // MAGDUR: expects multiple series (COUNT + STATUS + MIN_MAG + MAX_MAG + DURATION)
      if (count < 5) {
        defn.loggerCompliance.log('MAGDUR channel should have at least 5 series definitions (COUNT + STATUS + MIN_MAG + MAX_MAG + DURATION), found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idCount)) defn.loggerCompliance.log('MAGDUR series[0] should be COUNT.', LogLevels.Warning);
        if (!eq(vt(1), idStatus)) defn.loggerCompliance.log('MAGDUR series[1] should be STATUS.', LogLevels.Warning);
        if (!eq(vt(2), idMin)) defn.loggerCompliance.log('MAGDUR series[2] should be MIN.', LogLevels.Warning);
        if (!eq(vt(3), idMax)) defn.loggerCompliance.log('MAGDUR series[3] should be MAX.', LogLevels.Warning);
        if (!eq(vt(4), idDuration)) defn.loggerCompliance.log('MAGDUR series[4] should be DURATION.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_MAGDURTIME)) {
      // MAGDURTIME: expects multiple series (COUNT + STATUS + TIME + MIN_MAG + MAX_MAG + DURATION)
      if (count < 6) {
        defn.loggerCompliance.log('MAGDURTIME channel should have at least 6 series definitions, found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idCount)) defn.loggerCompliance.log('MAGDURTIME series[0] should be COUNT.', LogLevels.Warning);
        if (!eq(vt(1), idStatus)) defn.loggerCompliance.log('MAGDURTIME series[1] should be STATUS.', LogLevels.Warning);
        if (!eq(vt(2), idTime)) defn.loggerCompliance.log('MAGDURTIME series[2] should be TIME.', LogLevels.Warning);
        if (!eq(vt(3), idMin)) defn.loggerCompliance.log('MAGDURTIME series[3] should be MIN.', LogLevels.Warning);
        if (!eq(vt(4), idMax)) defn.loggerCompliance.log('MAGDURTIME series[4] should be MAX.', LogLevels.Warning);
        if (!eq(vt(5), idDuration)) defn.loggerCompliance.log('MAGDURTIME series[5] should be DURATION.', LogLevels.Warning);
      }
    } else if (guidEquals(qt, idGuids.ID_QT_MAGDURCOUNT)) {
      // MAGDURCOUNT: expects multiple series (COUNT + STATUS + MIN_MAG + MAX_MAG + DURATION + COUNT2)
      if (count < 6) {
        defn.loggerCompliance.log('MAGDURCOUNT channel should have at least 6 series definitions, found ' + count + '.', LogLevels.Warning);
      } else {
        if (!eq(vt(0), idCount)) defn.loggerCompliance.log('MAGDURCOUNT series[0] should be COUNT.', LogLevels.Warning);
        if (!eq(vt(1), idStatus)) defn.loggerCompliance.log('MAGDURCOUNT series[1] should be STATUS.', LogLevels.Warning);
        if (!eq(vt(2), idMin)) defn.loggerCompliance.log('MAGDURCOUNT series[2] should be MIN.', LogLevels.Warning);
        if (!eq(vt(3), idMax)) defn.loggerCompliance.log('MAGDURCOUNT series[3] should be MAX.', LogLevels.Warning);
        if (!eq(vt(4), idDuration)) defn.loggerCompliance.log('MAGDURCOUNT series[4] should be DURATION.', LogLevels.Warning);
        if (!eq(vt(5), idCount)) defn.loggerCompliance.log('MAGDURCOUNT series[5] should be COUNT.', LogLevels.Warning);
      }
    }
  }

  /**
   * Get XML representation.
   * @param {boolean} applicationLog
   * @param {boolean} complianceLog
   * @returns {string}
   */
  getXML(applicationLog, complianceLog) {
    let xml = '<tagOneChannelDefn>\r\n';
    if (applicationLog) xml += '<ApplicationLog>\r\n' + this.loggerApplication.getLogXML() + '</ApplicationLog>\r\n';
    if (complianceLog) xml += '<ComplianceLog>\r\n' + this.loggerCompliance.getLogXML() + '</ComplianceLog>\r\n';

    xml += xmlGetElement(tagGuids.tagChannelName, this.channelName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPhaseID, this.phaseID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagOtherChannelIdentifier, this.otherChannelIdentifier) + '\r\n';
    xml += xmlGetElement(tagGuids.tagGroupName, this.groupName) + '\r\n';
    if (this.quantityTypeID) xml += xmlGetElement(tagGuids.tagQuantityTypeID, this.quantityTypeID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagQuantityMeasuredID, this.quantityMeasuredID) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPhysicalChannel, this.physicalChannel) + '\r\n';
    xml += xmlGetElement(tagGuids.tagQuantityName, this.quantityName) + '\r\n';
    xml += xmlGetElement(tagGuids.tagPrimarySeriesIdx, this.primarySeriesIdx) + '\r\n';

    xml += '<tagSeriesDefns>\r\n';
    for (const sd of this.seriesDefns) {
      xml += sd.getXML(applicationLog, complianceLog);
    }
    xml += '</tagSeriesDefns>\r\n';

    xml += '</tagOneChannelDefn>\r\n';
    return xml;
  }

  /**
   * Get JSON representation.
   * @returns {Object}
   */
  getJSON() {
    return {
      channelName: this.channelName || '',
      phaseID: this.phaseID,
      otherChannelIdentifier: this.otherChannelIdentifier || '',
      groupName: this.groupName || '',
      quantityTypeID: this.quantityTypeID ? this.quantityTypeID.toString('hex') : null,
      quantityMeasuredID: this.quantityMeasuredID,
      physicalChannel: this.physicalChannel,
      quantityName: this.quantityName || '',
      primarySeriesIdx: this.primarySeriesIdx,
      seriesDefinitions: this.seriesDefns.map(sd => sd.getJSON())
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

function _getStringFromElement(el) {
  if (el && typeof el.getValues === 'function') {
    const out = { value: '' };
    el.getValues(out);
    return out.value;
  }
  if (el && typeof el.getString === 'function') return el.getString();
  return '';
}

module.exports = ChannelDefinition;
