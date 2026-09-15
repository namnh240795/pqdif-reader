'use strict';

/**
 * Base stream I/O class.
 * Ported from str_base.h / str_base.cpp
 *
 * Provides the interface and buffer management for reading/writing PQDIF data
 * through a connected StreamProcessor. Subclasses (FileStreamIO, ChunkStreamIO)
 * implement the actual I/O mechanics.
 */
class StreamIO {
  constructor() {
    /** @type {import('./streamProcessor').StreamProcessor|null} */
    this.processor = null;

    /** Whether full writes are supported */
    this.canWriteFull = false;
    /** Whether incremental writes are supported */
    this.canWriteInc = false;

    // Read buffer management
    /** @type {Buffer|null} */
    this.buffRead = Buffer.alloc(0);
    /** @type {number} Number of valid bytes in the read buffer */
    this.sizeRead = 0;
    /** @type {number} Current read position within the read buffer */
    this.posRead = 0;

    // Write buffer management
    /** @type {Buffer|null} */
    this.buffWrite = Buffer.alloc(0);
    /** @type {number} Number of valid bytes in the write buffer */
    this.sizeWrite = 0;
    /** @type {number} Current write position within the write buffer */
    this.posWrite = 0;
  }

  // -----------------------------------------------------------------------
  //  Pure virtual methods -- must be overridden by subclasses
  // -----------------------------------------------------------------------

  /**
   * Seek to an absolute position in the underlying storage.
   * @param {number} pos - Absolute byte offset
   * @returns {boolean} True on success
   */
  seekPos(pos) {
    throw new Error('StreamIO.seekPos() not implemented');
  }

  /**
   * Get the current absolute position in the underlying storage.
   * @returns {number} Current position
   */
  getPos() {
    throw new Error('StreamIO.getPos() not implemented');
  }

  /**
   * Seek to the end of the underlying storage.
   * @returns {boolean} True on success
   */
  seekEnd() {
    throw new Error('StreamIO.seekEnd() not implemented');
  }

  /**
   * Read a block of data from the underlying storage into the read buffer,
   * then run it through the connected processor's decode method.
   * @param {number} size - Number of bytes to read
   * @returns {{ data: Buffer, size: number }|null} Decoded data or null
   */
  readBlock(size) {
    throw new Error('StreamIO.readBlock() not implemented');
  }

  /**
   * Run the write buffer through the connected processor's encode method,
   * then flush the encoded data to the underlying storage.
   * @returns {boolean} True on success
   */
  writeBlock() {
    throw new Error('StreamIO.writeBlock() not implemented');
  }

  // -----------------------------------------------------------------------
  //  Optional overridables
  // -----------------------------------------------------------------------

  /**
   * Flush any buffered data to the underlying storage.
   * Default: no-op.
   */
  flush() {
    // No-op by default
  }

  // -----------------------------------------------------------------------
  //  Processor interface -- called by the connected StreamProcessor
  // -----------------------------------------------------------------------

  /**
   * Dereference data buffered in the stream for the processor to read.
   * Returns { data, size, pos } where:
   *   - data is the underlying Buffer
   *   - size is the number of remaining readable bytes (from pos to end)
   *   - pos is the current read cursor
   *
   * @returns {{ data: Buffer, size: number, pos: number }}
   */
  processRead() {
    const remaining = this.sizeRead - this.posRead;
    return {
      data: this.buffRead,
      size: remaining > 0 ? remaining : 0,
      pos: this.posRead,
    };
  }

  /**
   * Reserve storage in the write buffer for the processor to write into.
   * @param {number} size - Number of bytes to reserve
   * @returns {{ data: Buffer, pos: number }|null} Buffer reference and write position
   */
  processWriteReserve(size) {
    const needed = this.posWrite + size;
    if (!this.buffWrite || needed > this.buffWrite.length) {
      // Grow the buffer (at least double the current size, or enough for the needed space)
      const newSize = Math.max(needed, (this.buffWrite ? this.buffWrite.length * 2 : 0) + size);
      const newBuf = Buffer.alloc(newSize);
      if (this.buffWrite && this.posWrite > 0) {
        this.buffWrite.copy(newBuf, 0, 0, this.posWrite);
      }
      this.buffWrite = newBuf;
    }
    return { data: this.buffWrite, pos: this.posWrite };
  }

  /**
   * Release the write buffer after the processor has written data into it.
   * @param {number} sizeActual - Number of bytes actually written
   * @returns {boolean} True on success
   */
  processWriteRelease(sizeActual) {
    this.posWrite += sizeActual;
    this.sizeWrite = this.posWrite;
    return true;
  }

  // -----------------------------------------------------------------------
  //  Block accumulation -- for reading record bodies from chunks
  // -----------------------------------------------------------------------

  /**
   * Clear the read buffer and reset the read position.
   * Called before accumulating a new block of data.
   * @returns {boolean} Always true
   */
  beginBlock() {
    this.buffRead = Buffer.alloc(0);
    this.sizeRead = 0;
    this.posRead = 0;
    return true;
  }

  /**
   * Append data to the read buffer.
   * Used to accumulate data before processing (e.g., reading a record body).
   * @param {Buffer} buffer - Data to append
   * @param {number} size - Number of bytes to append
   * @returns {boolean} True on success
   */
  appendBlock(buffer, size) {
    const newBuf = Buffer.alloc(this.sizeRead + size);
    if (this.sizeRead > 0) {
      this.buffRead.copy(newBuf, 0, 0, this.sizeRead);
    }
    buffer.copy(newBuf, this.sizeRead, 0, size);
    this.buffRead = newBuf;
    this.sizeRead += size;
    return true;
  }

  // -----------------------------------------------------------------------
  //  Processor connection
  // -----------------------------------------------------------------------

  /**
   * Connect a StreamProcessor to this stream.
   * @param {import('./streamProcessor').StreamProcessor} processor
   * @returns {boolean} Always true
   */
  connectProcessor(processor) {
    this.processor = processor;
    processor.connectStream(this);
    return true;
  }

  /**
   * Get the Adler-32 checksum from the connected processor.
   * @returns {number}
   */
  getChecksum() {
    if (this.processor) {
      return this.processor.getChecksum();
    }
    return 0;
  }

  /**
   * Reset the Adler-32 checksum in the connected processor.
   */
  resetChecksum() {
    if (this.processor) {
      this.processor.resetChecksum();
    }
  }

  /**
   * Execute the connected processor's encode method.
   * Connects the processor to this stream first, then calls streamEncode().
   * @returns {boolean} True on success
   */
  executeProcessorEncode() {
    if (this.processor) {
      this.processor.connectStream(this);
      return this.processor.streamEncode();
    }
    return false;
  }

  /**
   * Execute the connected processor's decode method.
   * Connects the processor to this stream first, then calls streamDecode().
   * @returns {boolean} True on success
   */
  executeProcessorDecode() {
    if (this.processor) {
      this.processor.connectStream(this);
      return this.processor.streamDecode();
    }
    return false;
  }
}

module.exports = { StreamIO };
