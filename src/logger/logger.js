'use strict';

const fs = require('fs');
const LogLevels = require('./logLevels');
const LogEvent = require('./logEvent');

/**
 * For Managing Log Events
 */
class Logger {
  /**
   * Initializes a new instance of the Logger class
   */
  constructor() {
    /** Count of Log Events Saved by this Logger */
    this._logEventCount = 0;

    /** List of Log Messages Recorded with this Logger */
    this._logEvents = [];

    /** Specifies the Minimum Log Level Required to Save a Log Event */
    this.logLevel = LogLevels.Warning;

    /** If set to true, then each log message will be added to a list */
    this.logToList = true;

    /** Gets or sets the file name of the log file */
    this.logFileName = '';
  }

  /**
   * Gets the count of log events logged by this Logger
   * @returns {number} Count of Log Events
   */
  get eventLogCount() {
    return this._logEventCount;
  }

  /**
   * Gets the log events logged by this Logger
   * @returns {LogEvent[]} List of Log Events
   */
  getLogEventList() {
    return this._logEvents;
  }

  /**
   * Gets the log events logged by this Logger filtered by level
   * @param {number} logLevel - Only Log Events with a level equal to this will be returned
   * @returns {LogEvent[]} List of Log Events
   */
  getLogEventListByLevel(logLevel) {
    const returnsLogs = [];
    for (const logEvent of this._logEvents) {
      if (logEvent.level === logLevel) {
        returnsLogs.push(logEvent);
      }
    }
    return returnsLogs;
  }

  /**
   * Gets the log events logged by this Logger formatted as XML records
   * @returns {string} Log Events Formatted in XML
   */
  getLogXML() {
    let xml = '';
    for (const logEvent of this._logEvents) {
      const levelName = Object.keys(LogLevels).find(key => LogLevels[key] === logEvent.level) || 'Unknown';
      xml += `<LogEvent type="${levelName}">${this._htmlEncode(logEvent.message)}</LogEvent>\r\n`;
    }
    return xml;
  }

  /**
   * Gets the log events logged by this Logger formatted as XML records, filtered by level
   * @param {number} logLevel - Only Log Events with a level equal to this will be returned
   * @returns {string} Log Events Formatted in XML
   */
  getLogXMLByLevel(logLevel) {
    let xml = '';
    for (const logEvent of this._logEvents) {
      if (logEvent.level === logLevel) {
        const levelName = Object.keys(LogLevels).find(key => LogLevels[key] === logEvent.level) || 'Unknown';
        xml += `<LogEvent type="${levelName}">${this._htmlEncode(logEvent.message)}</LogEvent>\r\n`;
      }
    }
    return xml;
  }

  /**
   * Gets the log events logged by this Logger equal to or after the specified start date
   * @param {Date} startDate - Start Date for Logs
   * @returns {LogEvent[]} List of Log Events
   */
  getLogEventListByDate(startDate) {
    return this._logEvents.filter(x => x.whenLogged >= startDate);
  }

  /**
   * Logs the specified LogEvent
   * @param {LogEvent} logEvent - Log Event to Log
   * @returns {boolean} true if the log was processed, false otherwise
   */
  logEvent(logEvent) {
    let logEventProcessed = false;

    // If the log message is high enough in priority to save, then process it
    if (logEvent.level <= this.logLevel) {
      logEventProcessed = this._processLogEvent(logEvent);
    }

    return logEventProcessed;
  }

  /**
   * Logs the specified message
   * @param {string} message - Text Message to Log
   * @param {number} logLevelForMessage - Log Level for Message
   * @returns {boolean} true if the log was processed, false otherwise
   */
  log(message, logLevelForMessage) {
    let logEventProcessed = false;

    // If the log message is high enough in priority to save, then process it
    if (logLevelForMessage <= this.logLevel) {
      const logEvent = new LogEvent(message, logLevelForMessage);
      logEventProcessed = this._processLogEvent(logEvent);
    }

    return logEventProcessed;
  }

  /**
   * Logs the specified message with an exception
   * @param {string} message - Text Message to Log
   * @param {number} logLevelForMessage - Log Level for Message
   * @param {Error} exception - Exception Associated with the Event Log
   * @returns {boolean} true if the log was processed, false otherwise
   */
  logMessage(message, logLevelForMessage, exception) {
    let logEventProcessed = false;

    // If the log message is high enough in priority to save, then process it
    if (logLevelForMessage <= this.logLevel) {
      const logEvent = new LogEvent(message, logLevelForMessage, exception);
      logEventProcessed = this._processLogEvent(logEvent);
    }

    return logEventProcessed;
  }

  /**
   * Processes the log event
   * @param {LogEvent} logEvent - Log Event to Process
   * @returns {boolean} true if the log was processed, false otherwise
   * @private
   */
  _processLogEvent(logEvent) {
    let logEventProcessed = false;

    try {
      // Initialize the log message
      const timestamp = logEvent.whenLogged.toISOString().replace('T', ' ').substring(0, 23);
      let logMessage = `${timestamp}: ${logEvent.message}`;
      if (logEvent.exception) {
        logMessage += ` - Exception Message: ${logEvent.exception.message}`;
      }

      // Increment the log event count
      this._logEventCount += 1;

      // If a file name is specified, write to the log file
      if (this.logFileName && this.logFileName.length > 0) {
        try {
          fs.appendFileSync(this.logFileName, logMessage + '\n');
          logEventProcessed = true;
        } catch (err) {
          // Ignore errors while logging to file
        }
      }

      // If we need to add the event to the list of log events
      if (this.logToList) {
        this._logEvents.push(logEvent);
        logEventProcessed = true;
      }
    } catch (err) {
      // Ignore errors while logging
    }

    return logEventProcessed;
  }

  /**
   * Adds a list of LogEvent records to the event log
   * @param {LogEvent[]} logs - The logs to add
   */
  logsAdd(logs) {
    this._logEvents.push(...logs);
  }

  /**
   * Clears the Event Log
   */
  logsClear() {
    this._logEvents = [];
  }

  /**
   * HTML-encodes a string
   * @param {string} text - The text to encode
   * @returns {string} The encoded text
   * @private
   */
  _htmlEncode(text) {
    return text
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;')
      .replace(/'/g, '&#39;');
  }
}

module.exports = Logger;
