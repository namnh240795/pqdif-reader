'use strict';

/**
 * Log Levels for the PQDIF Logger
 */
const LogLevels = {
  /** No messages are logged */
  Off: 0,

  /** Logs messages that presumably could be associated with the application aborting */
  Fatal: 1,

  /** Logs messages associated with exceptions */
  Error: 2,

  /** Logs messages that need attention but did not result in an exception */
  Warning: 3,

  /** Logs informational messages that highlight the progress of the application at coarse-grained level */
  Info: 4,

  /** Logs fine-grained informational events that are most useful to debug an application */
  Debug: 5,

  /** Logs all messages */
  All: 6
};

module.exports = LogLevels;
