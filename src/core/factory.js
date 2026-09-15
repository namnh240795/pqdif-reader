'use strict';

const { Collection } = require('./collection');
const { Scalar } = require('./scalar');
const { Vector } = require('./vector');
const { Record } = require('./record');
const { ContainerRecord } = require('./containerRecord');
const { DataSourceRecord } = require('./dataSourceRecord');
const { SettingsRecord } = require('./settingsRecord');
const { ObservationRecord } = require('./observationRecord');
const {
  ID_ELEMENT_TYPE_COLLECTION, ID_ELEMENT_TYPE_SCALAR, ID_ELEMENT_TYPE_VECTOR,
} = require('../constants/elementTypes');
const { tagContainer, tagRecDataSource, tagRecMonitorSettings, tagRecObservation } = require('../constants/tagGuids');

/**
 * CPQDIF_Factory - singleton factory for creating PQDIF objects.
 * Ported from pqdfacty.h/pqdfacty.cpp
 */
class Factory {
  /**
   * Create a new element of the specified type.
   * @param {number} elementType - 1=Collection, 2=Scalar, 3=Vector
   * @returns {Element|null}
   */
  newElement(elementType) {
    switch (elementType) {
      case ID_ELEMENT_TYPE_COLLECTION:
        return new Collection();
      case ID_ELEMENT_TYPE_SCALAR:
        return new Scalar();
      case ID_ELEMENT_TYPE_VECTOR:
        return new Vector();
      default:
        throw new Error(`Unknown element type: ${elementType}`);
    }
  }

  /**
   * Create a new record with the appropriate header tag.
   * @param {string} which - 'Record', 'Container', 'DataSource', 'MonitorSettings', 'Observation'
   * @returns {Record|null}
   */
  newRecord(which) {
    let prec = null;

    switch (which) {
      case 'Record':
        prec = new Record();
        prec.setHeaderTag(null);
        break;
      case 'Container':
        prec = new ContainerRecord();
        prec.setHeaderTag(tagContainer);
        break;
      case 'DataSource':
        prec = new DataSourceRecord();
        prec.setHeaderTag(tagRecDataSource);
        break;
      case 'MonitorSettings':
        prec = new SettingsRecord();
        prec.setHeaderTag(tagRecMonitorSettings);
        break;
      case 'Observation':
        prec = new Record();
        prec.setHeaderTag(tagRecObservation);
        break;
      default:
        prec = null;
        break;
    }

    return prec;
  }

  /**
   * Create an observation wrapper around a base record with a data source reference.
   * @param {Record} precBase
   * @param {DataSourceRecord} precDataSource
   * @returns {ObservationRecord|null}
   */
  newObservationWrapper(precBase, precDataSource) {
    const pobs = new ObservationRecord(precBase);
    if (pobs && precDataSource) {
      pobs.setDataSource(precDataSource);
    }
    return pobs;
  }

  /**
   * Create an observation wrapper with both data source and settings references.
   * @param {Record} precBase
   * @param {DataSourceRecord} precDataSource
   * @param {SettingsRecord} precSettings
   * @returns {ObservationRecord|null}
   */
  newObservationWrapper2(precBase, precDataSource, precSettings) {
    const pobs = new ObservationRecord(precBase);
    if (pobs) {
      if (precDataSource) pobs.setDataSource(precDataSource);
      if (precSettings) pobs.setMonitorSettings(precSettings);
    }
    return pobs;
  }
}

// Singleton export
module.exports = new Factory();
