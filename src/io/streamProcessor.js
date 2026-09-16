'use strict';

const pako = require('pako');

/**
 * Compute Adler-32 checksum (matching zlib's adler32).
 * @param {Buffer} data - Input data
 * @param {number} [prev=0] - Previous checksum value (for chaining)
 * @returns {number} Adler-32 checksum
 */
function adler32(data, prev = 0) {
  const MOD = 65521;
  let a = prev & 0xFFFF;
  let b = (prev >>> 16) & 0xFFFF;

  for (let i = 0; i < data.length; i++) {
    a = (a + data[i]) % MOD;
    b = (b + a) % MOD;
  }

  return ((b << 16) | a) >>> 0;
}

/**
 * Base stream processor class.
 * Ported from proc_bas.h / proc_bas.cpp
 *
 * The processor sits between a StreamIO and the raw data, performing
 * optional compression / decompression and maintaining an Adler-32 checksum.
 */
class StreamProcessor {
  constructor() {
    this.stream = null;
    this.checksum = 1; // zlib adler32 initial value is 1, not 0
  }

  /**
   * Connect a StreamIO to this processor.
   * @param {import('./streamIO').StreamIO} stream
   */
  connectStream(stream) {
    this.stream = stream;
  }

  /**
   * @returns {number} Current Adler-32 checksum value
   */
  getChecksum() {
    return this.checksum;
  }

  /**
   * Reset the Adler-32 checksum to its initial value.
   */
  resetChecksum() {
    this.checksum = 1; // zlib adler32 initial value
  }

  /**
   * Encode (compress) data from the stream's read buffer into its write buffer.
   * Must be overridden by subclasses.
   */
  streamEncode() {
    throw new Error('StreamProcessor.streamEncode() not implemented');
  }

  /**
   * Decode (decompress) data from the stream's read buffer into its write buffer.
   * Must be overridden by subclasses.
   */
  streamDecode() {
    throw new Error('StreamProcessor.streamDecode() not implemented');
  }
}

/**
 * "Do nothing" processor - passes data through unchanged.
 * Used when the PQDIF file is not compressed, or for records which are
 * never compressed (e.g. the container record header).
 *
 * Ported from proc_not.h / proc_not.cpp
 */
class NothingProcessor extends StreamProcessor {
  /**
   * Read input from the connected stream, write it to the output unchanged,
   * and update the Adler-32 checksum over the input data.
   */
  streamEncode() {
    // ProcessRead returns { data, size, pos } -- data is the buffer,
    // size is the number of readable bytes, pos is the read cursor.
    const input = this.stream.processRead();
    if (!input || input.size === 0) return true;

    // Reserve space in the output buffer
    const output = this.stream.processWriteReserve(input.size);
    if (!output) return false;

    // Copy input to output
    input.data.copy(output.data, output.pos, input.pos, input.pos + input.size);

    // Update checksum over the raw input data (not the output)
    const slice = input.data.subarray(input.pos, input.pos + input.size);
    this.checksum = adler32(slice, this.checksum);

    // Release the written bytes
    this.stream.processWriteRelease(input.size);
    return true;
  }

  /**
   * Decoding is the same as encoding for the Nothing processor.
   */
  streamDecode() {
    return this.streamEncode();
  }
}

/**
 * ZLIB compression / decompression processor.
 * Uses pako for pure-JS zlib implementation.
 *
 * Ported from proc_zlib.h / proc_zlib.cpp
 */
class ZlibProcessor extends StreamProcessor {
  /**
   * Compress the input data using ZLIB (deflate) at best compression.
   */
  streamEncode() {
    const input = this.stream.processRead();
    if (!input || input.size === 0) return true;

    const rawData = input.data.subarray(input.pos, input.pos + input.size);

    // Compress with best compression level (matching C++ Z_BEST_COMPRESSION)
    let compressed;
    try {
      compressed = Buffer.from(pako.deflate(rawData, { level: 9 }));
    } catch (e) {
      throw new Error(`ZLIB compression failed: ${e.message}`);
    }

    // Reserve output space and write compressed data
    const output = this.stream.processWriteReserve(compressed.length);
    if (!output) return false;

    compressed.copy(output.data, output.pos);

    // Update checksum over the compressed output data
    this.checksum = adler32(compressed, this.checksum);

    // Release the written bytes
    this.stream.processWriteRelease(compressed.length);
    return true;
  }

  /**
   * Decompress the input data using ZLIB (inflate).
   * Uses a loop similar to the C++ implementation which may call inflate
   * multiple times if the output buffer is not large enough.
   */
  streamDecode() {
    const input = this.stream.processRead();
    if (!input || input.size === 0) return true;

    const rawData = input.data.subarray(input.pos, input.pos + input.size);

    // Initial output buffer size: max(input*4, 32KB) -- matching C++ logic
    let outputSize = Math.max(input.size * 4, 32 * 1024);

    let decompressed;
    try {
      decompressed = Buffer.from(pako.inflate(rawData));
    } catch (e) {
      throw new Error(`ZLIB decompression failed: ${e.message}`);
    }

    // Reserve output space and write decompressed data
    const output = this.stream.processWriteReserve(decompressed.length);
    if (!output) return false;

    decompressed.copy(output.data, output.pos);

    // Update checksum over the decompressed output data
    this.checksum = adler32(decompressed, this.checksum);

    // Release the written bytes
    this.stream.processWriteRelease(decompressed.length);
    return true;
  }
}

module.exports = { StreamProcessor, NothingProcessor, ZlibProcessor };
