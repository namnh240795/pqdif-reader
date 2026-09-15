'use strict';

const fs = require('fs');
const { StreamIO } = require('./streamIO');

/**
 * File-based stream I/O using Node.js fs.
 * Ported from str_flat.h / str_flat.cpp
 *
 * Supports opening files in read-only or read/write mode.
 * The file pointer is tracked internally and advanced by read/write operations.
 */
class FileStreamIO extends StreamIO {
  constructor() {
    super();

    /** @type {number|null} File descriptor */
    this.fd = null;

    /** @type {string} Path to the file */
    this.fileName = '';

    /** @type {number} Current file position (mirrors fd position) */
    this.filePos = 0;
  }

  // -----------------------------------------------------------------------
  //  File operations
  // -----------------------------------------------------------------------

  /**
   * Open an existing file for reading or reading+writing.
   * @param {string} fileName - Path to the file
   * @param {boolean} readOnly - If true, open read-only; otherwise read/write
   * @returns {boolean} True on success
   */
  open(fileName, readOnly) {
    if (this.fd !== null) {
      return false; // Already open
    }

    this.fileName = fileName;

    if (readOnly) {
      this.fd = fs.openSync(fileName, 'r');
      this.canWriteFull = false;
      this.canWriteInc = false;
    } else {
      this.fd = fs.openSync(fileName, 'r+');
      this.canWriteFull = true;
      this.canWriteInc = true;
    }

    this.filePos = 0;
    return true;
  }

  /**
   * Create a new file for read+write (truncates if existing).
   * @param {string} fileName - Path to the file
   * @returns {boolean} True on success
   */
  createNew(fileName) {
    if (this.fd !== null) {
      return false; // Already open
    }

    this.fileName = fileName;
    this.canWriteFull = true;
    this.canWriteInc = true;

    this.fd = fs.openSync(fileName, 'w+');
    this.filePos = 0;
    return true;
  }

  /**
   * Close the file if open.
   */
  close() {
    if (this.fd !== null) {
      fs.closeSync(this.fd);
      this.fd = null;
    }
  }

  // -----------------------------------------------------------------------
  //  StreamIO overrides
  // -----------------------------------------------------------------------

  /**
   * Seek to an absolute position in the file.
   * @param {number} pos - Absolute byte offset
   * @returns {boolean} True on success
   */
  seekPos(pos) {
    if (this.fd === null) return false;
    this.filePos = pos;
    return true;
  }

  /**
   * Get the current absolute position in the file.
   * @returns {number} Current position
   */
  getPos() {
    return this.filePos;
  }

  /**
   * Seek to the end of the file.
   * @returns {boolean} True on success
   */
  seekEnd() {
    if (this.fd === null) return false;
    const stats = fs.fstatSync(this.fd);
    this.filePos = stats.size;
    return true;
  }

  /**
   * Read a block of data from the file into the read buffer,
   * then decode it through the connected processor.
   *
   * Matches C++ CPQDIF_S_FlatFile::ReadBlock behavior:
   * 1. Clear read/write buffers
   * 2. Read raw bytes from file
   * 3. Run through processor's decode
   * 4. Return decoded data via the write buffer
   *
   * @param {number} size - Number of bytes to read
   * @returns {{ data: Buffer, size: number }|null} Decoded data or null at EOF
   */
  readBlock(size) {
    if (this.fd === null) return null;

    // Clear buffers (matching C++ behavior)
    this.buffRead = Buffer.alloc(0);
    this.posRead = 0;
    this.buffWrite = Buffer.alloc(0);
    this.posWrite = 0;
    this.sizeRead = 0;
    this.sizeWrite = 0;

    // Check for EOF
    const stats = fs.fstatSync(this.fd);
    if (this.filePos >= stats.size) {
      return null;
    }

    // Read raw bytes from file
    const rawBuf = Buffer.alloc(size);
    const bytesRead = fs.readSync(this.fd, rawBuf, 0, size, this.filePos);
    this.filePos += bytesRead;

    if (bytesRead <= 0) {
      return null;
    }

    // Set up the read buffer with the raw data
    this.buffRead = rawBuf.subarray(0, bytesRead);
    this.sizeRead = bytesRead;
    this.posRead = 0;

    // Decode through processor
    const status = this.executeProcessorDecode();
    if (status) {
      return {
        data: this.buffWrite,
        size: this.posWrite,
      };
    }

    return null;
  }

  /**
   * Encode the write buffer through the connected processor,
   * then write the encoded data to the file.
   *
   * Matches C++ CPQDIF_S_FlatFile::WriteBlock behavior:
   * 1. Clear write buffer
   * 2. Run through processor's encode (reads from read buffer)
   * 3. Write encoded data to file
   *
   * @returns {boolean} True on success
   */
  writeBlock() {
    if (this.fd === null) return false;

    // Clear write buffer
    this.buffWrite = Buffer.alloc(0);
    this.posWrite = 0;
    this.sizeWrite = 0;

    // Encode through processor
    const status = this.executeProcessorEncode();
    if (!status) return false;

    // Write the encoded data to the file
    const sizeActual = this.posWrite;
    if (sizeActual > 0) {
      const written = fs.writeSync(this.fd, this.buffWrite, 0, sizeActual, this.filePos);
      this.filePos += written;
      return written === sizeActual;
    }

    return true;
  }

  /**
   * Flush the file to disk.
   */
  flush() {
    if (this.fd !== null) {
      fs.fsyncSync(this.fd);
    }
  }
}

module.exports = { FileStreamIO };
