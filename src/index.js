'use strict';

/**
 * PQDIF Reader/Writer for Node.js
 * IEEE Std 1159.3 Power Quality Data Interchange Format
 */

// Core classes
const { Collection, Scalar, Vector, Record, ContainerRecord, DataSourceRecord,
        ObservationRecord, SettingsRecord, PqdifValue, factory } = require('./core');

// I/O
const { FileStreamIO, ChunkStreamIO, NothingProcessor, ZlibProcessor } = require('./io');

// Persistence
const { FlatFileController } = require('./persistence');

// Validator
const { PqFile, RecType } = require('./validator');

// Logger
const { Logger, LogEvent, LogLevels } = require('./logger');

// Constants
const constants = require('./constants');

// Info
const { PqdifInfo } = require('./info');

// Examples
const { Writer } = require('./examples');

module.exports = {
  // Main high-level API
  PqFile,
  RecType,

  // Core classes
  Collection,
  Scalar,
  Vector,
  Record,
  ContainerRecord,
  DataSourceRecord,
  ObservationRecord,
  SettingsRecord,
  PqdifValue,
  factory,

  // I/O
  FileStreamIO,
  ChunkStreamIO,
  NothingProcessor,
  ZlibProcessor,

  // Persistence
  FlatFileController,

  // Logger
  Logger,
  LogEvent,
  LogLevels,

  // Info/Constants
  PqdifInfo,
  constants,

  // Examples
  Writer
};
