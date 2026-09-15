'use strict';

const LogLevels = require('./logLevels');

/**
 * For storing a log event
 */
class LogEvent {
  /**
   * Initializes a new instance of the LogEvent class
   * @param {string} message - Text message associated with the event
   * @param {number} level - Log level for this message
   * @param {Error|null} exception - Exception associated with the event (if any)
   * @param {Date|null} whenLogged - Date and time when the event was logged (UTC)
   */
  constructor(message = '', level = LogLevels.Info, exception = null, whenLogged = null) {
    /** Date and Time when the Event was Logged (UTC) */
    this.whenLogged = whenLogged || new Date();

    /** Text Message associated with the Event */
    this.message = message;

    /** Exception Associated with the Event (if any) */
    this.exception = exception;

    /** Log Level for this Message */
    this.level = level;
  }
}

module.exports = LogEvent;
