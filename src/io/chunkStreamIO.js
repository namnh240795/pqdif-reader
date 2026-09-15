'use strict';

const { StreamIO } = require('./streamIO');

/**
 * In-memory chunk stream I/O.
 * Ported from str_chnk.h / str_chnk.cpp
 *
 * Supports reading from an in-memory buffer (chunk) and writing to an
 * in-memory output buffer. Useful for embedding PQDIF data in database
 * binary fields or for in-memory processing.
 */
class ChunkStreamIO extends StreamIO {
  constructor() {
    super();

    /** @type {Buffer|null} Input chunk (not owned) */
    this.chunkRead = null;

    /** @type {number} Size of the input chunk in bytes */
    this.sizeChunkRead = 0;

    /** @type {number} Current position within the chunk */
    this.posChunk = 0;

    /** @type {Buffer} Output chunk (grows as needed) */
    this.chunkWrite = Buffer.alloc(0);

    /** @type {number} Current write position in the output chunk */
    this.posChunkWrite = 0;
  }

  // -----------------------------------------------------------------------
  //  Chunk-specific operations
  // -----------------------------------------------------------------------

  /**
   * Set the input chunk to read from.
   * @param {Buffer} chunk - The input data buffer
   * @param {number} [size] - Number of valid bytes (defaults to chunk.length)
   * @returns {boolean} True on success
   */
  setInput(chunk, size) {
    if (chunk && size > 0) {
      this.chunkRead = chunk;
      this.sizeChunkRead = size;
      this.posChunk = 0;
      return true;
    }

    this.chunkRead = null;
    this.sizeChunkRead = 0;
    return false;
  }

  /**
   * Get the output chunk size.
   * @returns {number} Size of the output data
   */
  getOutputSize() {
    return this.posChunkWrite;
  }

  /**
   * Copy the output chunk into the provided buffer.
   * @param {Buffer} chunk - Destination buffer
   * @param {number} maxSize - Maximum bytes to copy
   * @returns {boolean} True on success
   */
  getOutput(chunk, maxSize) {
    if (!chunk || maxSize <= 0) return false;

    const sizeActual = Math.min(maxSize, this.posChunkWrite);
    this.chunkWrite.copy(chunk, 0, 0, sizeActual);
    return true;
  }

  /**
   * Get the output chunk buffer directly.
   * @returns {Buffer}
   */
  getOutputBuffer() {
    return this.chunkWrite.subarray(0, this.posChunkWrite);
  }

  // -----------------------------------------------------------------------
  //  StreamIO overrides
  // -----------------------------------------------------------------------

  /**
   * Seek to an absolute position within the chunk.
   * @param {number} pos - Byte offset
   * @returns {boolean} True on success
   */
  seekPos(pos) {
    this.posChunk = pos;
    return true;
  }

  /**
   * Get the current position within the chunk.
   * @returns {number} Current position
   */
  getPos() {
    return this.posChunk;
  }

  /**
   * Seek to the end of the current chunk (input or output, depending on mode).
   * @returns {boolean} True on success
   */
  seekEnd() {
    if (this.chunkRead) {
      this.posChunk = this.sizeChunkRead;
    } else {
      this.posChunk = this.posChunkWrite;
    }
    return true;
  }

  /**
   * Read a block from the input chunk into the read buffer,
   * then decode it through the connected processor.
   *
   * Matches C++ CPQDIF_S_Chunk::ReadBlock behavior:
   * 1. Clear read/write buffers
   * 2. Copy bytes from the chunk at posChunk
   * 3. Advance posChunk
   * 4. Run through processor's decode
   * 5. Return decoded data via the write buffer
   *
   * @param {number} size - Number of bytes to read
   * @returns {{ data: Buffer, size: number }|null} Decoded data or null
   */
  readBlock(size) {
    // Clear buffers (matching C++ behavior)
    this.buffRead = Buffer.alloc(0);
    this.posRead = 0;
    this.buffWrite = Buffer.alloc(0);
    this.posWrite = 0;
    this.sizeRead = 0;
    this.sizeWrite = 0;

    // Check for valid input
    if (!this.chunkRead || this.sizeChunkRead <= 0 || this.posChunk < 0) {
      return null;
    }

    // Calculate how much we can read
    const sizeAvailable = this.sizeChunkRead - this.posChunk;
    if (sizeAvailable <= 0) {
      return null;
    }

    const sizeRead = Math.min(size, sizeAvailable);

    // Copy data from the chunk to the read buffer
    this.buffRead = Buffer.alloc(sizeRead);
    this.chunkRead.copy(this.buffRead, 0, this.posChunk, this.posChunk + sizeRead);
    this.sizeRead = sizeRead;
    this.posRead = 0;

    // Advance the chunk position
    this.posChunk += sizeRead;

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
   * then append the encoded data to the output chunk.
   *
   * Matches C++ CPQDIF_S_Chunk::WriteBlock behavior:
   * 1. Clear write buffer
   * 2. Run through processor's encode
   * 3. Resize output chunk if needed
   * 4. Copy encoded data at posChunk
   * 5. Advance posChunk
   *
   * @returns {boolean} True on success
   */
  writeBlock() {
    // Clear write buffer
    this.buffWrite = Buffer.alloc(0);
    this.posWrite = 0;
    this.sizeWrite = 0;

    // Encode through processor
    const status = this.executeProcessorEncode();
    if (!status) return false;

    const sizeActual = this.posWrite;
    if (sizeActual <= 0) return true;

    // Resize the output chunk if needed
    const sizeNew = this.posChunk + sizeActual;
    if (sizeNew > this.chunkWrite.length) {
      // Grow: at least double, or exactly the needed size
      const newSize = Math.max(sizeNew, this.chunkWrite.length * 2);
      const newBuf = Buffer.alloc(newSize);
      if (this.posChunkWrite > 0) {
        this.chunkWrite.copy(newBuf, 0, 0, this.posChunkWrite);
      }
      this.chunkWrite = newBuf;
    }

    // Copy encoded data into the output chunk at the current position
    this.buffWrite.copy(this.chunkWrite, this.posChunk, 0, sizeActual);

    // Advance positions
    this.posChunk += sizeActual;
    this.posChunkWrite = this.posChunk;

    return true;
  }
}

module.exports = { ChunkStreamIO };
