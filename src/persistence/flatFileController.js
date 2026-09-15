'use strict';

const fs = require('fs');
const PersistenceController = require('./controller');
const { FileStreamIO } = require('../io/fileStreamIO');
const { ChunkStreamIO } = require('../io/chunkStreamIO');
const { NothingProcessor, ZlibProcessor } = require('../io/streamProcessor');
const { PQController } = require('../parsing/parser');
const { RECORD_HEADER_SIZE } = require('../constants/physicalTypes');
const { ID_COMP_ALG_NONE, ID_COMP_ALG_ZLIB, ID_COMP_STYLE_NONE, ID_COMP_STYLE_RECORDLEVEL, ID_COMP_STYLE_TOTALFILE } = require('../constants/compressionIds');
const { PqdifInfo } = require('../info/info');
const tagGuids = require('../constants/tagGuids');

/**
 * Flat file persistence controller.
 * Ported from pcn_flat.h / pcn_flat.cpp
 *
 * Implements reading and writing PQDIF files on disk or in memory chunks.
 * Handles compression/decompression at the record level using the
 * StreamProcessor / StreamIO infrastructure.
 */
class FlatFileController extends PersistenceController {
  constructor() {
    super();

    /** @type {string} Path to the PQDIF file */
    this.fileName = '';

    /** @type {Buffer|null} Input chunk (for in-memory mode) */
    this.chunk = null;

    /** @type {number} Size of the input chunk */
    this.sizeChunk = 0;

    /** @type {import('../io/streamIO').StreamIO|null} Active stream */
    this.stream = null;

    /** @type {import('../io/streamProcessor').StreamProcessor|null} Processor for record headers */
    this.processorHeader = null;

    /** @type {import('../io/streamProcessor').StreamProcessor|null} Processor for record bodies */
    this.processorBody = null;

    /** @type {number} Current compression algorithm */
    this.compressionAlgorithm = ID_COMP_ALG_NONE;

    /** @type {number} Current compression style */
    this.compressionStyle = ID_COMP_STYLE_NONE;
  }

  // -----------------------------------------------------------------------
  //  File name management
  // -----------------------------------------------------------------------

  setFileName(fname) {
    this.fileName = fname;
  }

  getFileName() {
    return this.fileName;
  }

  // -----------------------------------------------------------------------
  //  Chunk I/O management
  // -----------------------------------------------------------------------

  /**
   * Set the input chunk for in-memory processing.
   * @param {Buffer} chunk
   * @param {number} size
   */
  setChunkInput(chunk, size) {
    if (chunk && size > 0) {
      this.chunk = chunk;
      this.sizeChunk = size;
    } else {
      this.chunk = null;
      this.sizeChunk = 0;
    }
  }

  /**
   * Get the output chunk size after writing.
   * @returns {number}
   */
  getChunkOutputSize() {
    if (this.stream && this.stream instanceof ChunkStreamIO) {
      return this.stream.getOutputSize();
    }
    return 0;
  }

  /**
   * Get the output chunk data after writing.
   * @param {Buffer} dest - Destination buffer
   * @param {number} size - Max bytes to copy
   * @returns {boolean}
   */
  getChunkOutput(dest, size) {
    if (this.stream && this.stream instanceof ChunkStreamIO) {
      return this.stream.getOutput(dest, size);
    }
    return false;
  }

  // -----------------------------------------------------------------------
  //  Compression management
  // -----------------------------------------------------------------------

  getCompressionAlgorithm() {
    return this.compressionAlgorithm;
  }

  setCompressionAlgorithm(alg) {
    this.compressionAlgorithm = alg;
  }

  getCompressionStyle() {
    return this.compressionStyle;
  }

  setCompressionStyle(style) {
    this.compressionStyle = style;
  }

  // -----------------------------------------------------------------------
  //  Read operations
  // -----------------------------------------------------------------------

  /**
   * Read headers only from a file (fast initial read).
   * This reads the first record (container) to determine compression settings,
   * then reads headers of all subsequent records.
   *
   * Ported from C++ CPQDIF_PC_FlatFile::ReadHeaders()
   *
   * @returns {boolean} True on success
   */
  readHeaders() {
    let status = false;

    // Clean up old stream/processors
    this._cleanup();

    // Determine stream type
    const useChunk = this.chunk !== null;

    // Create stream and processors
    if (useChunk) {
      this.stream = new ChunkStreamIO();
      this.stream.setInput(this.chunk, this.sizeChunk);
    } else {
      this.stream = new FileStreamIO();
      if (!this.stream.open(this.fileName, true)) {
        return false;
      }
    }

    this.processorHeader = new NothingProcessor();
    this.processorBody = new NothingProcessor();

    // Read the first record header + body (using NothingProcessor)
    this.stream.connectProcessor(this.processorHeader);

    // Read first record header
    const headerResult = this.stream.readBlock(RECORD_HEADER_SIZE);
    if (!headerResult) return false;

    const headerBuf = headerResult.data;
    const headerSize = headerResult.size;

    // Parse the record header to get sizes
    // Header format (64 bytes total):
    //   0-15:  guidRecordSignature (16 bytes)
    //  16-31:  tagRecordType (16 bytes)
    //  32-35:  sizeHeader (4 bytes)
    //  36-39:  sizeData (4 bytes)
    //  40-43:  linkNextRecord (4 bytes)
    //  44-47:  checksum (4 bytes)
    //  48-63:  reserved (16 bytes)
    const sizeHeader = headerBuf.readInt32LE(32);
    const sizeData = headerBuf.readInt32LE(36);

    // Read the first record body
    this.stream.connectProcessor(this.processorBody);
    const bodyResult = this.stream.readBlock(sizeData);
    if (!bodyResult) return false;

    const bodyBuf = bodyResult.data;
    const bodySize = bodyResult.size;

    // Parse the container record body to extract compression info
    status = this._parseContainerCompression(bodyBuf, bodySize);

    if (status) {
      // Read through remaining record headers
      let posNext = headerBuf.readInt32LE(40); // linkNextRecord

      while (status) {
        if (posNext <= 0 || posNext >= this.sizeChunk) {
          break;
        }

        this.stream.seekPos(posNext);
        this.stream.connectProcessor(this.processorHeader);

        const nextHeaderResult = this.stream.readBlock(RECORD_HEADER_SIZE);
        if (!nextHeaderResult) break;

        // We don't create full Record objects here, just track headers
        posNext = nextHeaderResult.data.readInt32LE(40);
      }
    }

    this.state = 'empty';
    return status;
  }

  /**
   * Import container and all records from a chunk (in-memory buffer).
   *
   * Ported from C++ CPQDIF_PC_FlatFile::ImportContainerAndRecordsFromChunk()
   *
   * @param {Buffer} chunk - The PQDIF data buffer
   * @param {number} size - Size of the buffer
   * @returns {{ status: boolean, compressed: boolean }} Result with compression flag
   */
  importContainerAndRecordsFromChunk(chunk, size) {
    let status = false;
    let bCompressed = false;

    // Clean up old stream/processors
    this._cleanup();

    if (!chunk || size <= 0) {
      return { status: false, compressed: false };
    }

    // Create stream and default processors
    this.stream = new ChunkStreamIO();
    this.processorHeader = new NothingProcessor();
    this.processorBody = new NothingProcessor();

    // Open the stream
    this.stream.setInput(chunk, size);

    // Read first header with Nothing processor
    this.stream.connectProcessor(this.processorHeader);

    // Read first record header
    const headerResult = this.stream.readBlock(RECORD_HEADER_SIZE);
    if (!headerResult) {
      return { status: false, compressed: false };
    }

    const headerBuf = headerResult.data;
    const sizeHeader = headerBuf.readInt32LE(32);
    const sizeData = headerBuf.readInt32LE(36);
    let posNext = headerBuf.readInt32LE(40);

    // Read the first record body
    this.stream.connectProcessor(this.processorBody);
    const bodyResult = this.stream.readBlock(sizeData);
    if (!bodyResult) {
      return { status: false, compressed: false };
    }

    // Parse container compression info
    const foundCompInfo = this._parseContainerCompression(bodyResult.data, bodyResult.size);

    if (foundCompInfo) {
      switch (this.compressionStyle) {
        case ID_COMP_STYLE_TOTALFILE:
          // Unsupported
          return { status: false, compressed: false };

        case ID_COMP_STYLE_RECORDLEVEL:
          // Switch to the appropriate body processor
          this.processorBody = this._createProcessor(this.compressionAlgorithm);
          if (this.processorBody) {
            bCompressed = true;
          } else {
            return { status: false, compressed: false };
          }
          break;

        case ID_COMP_STYLE_NONE:
        default:
          // No compression
          break;
      }
    }

    status = true;

    // Read through remaining record headers
    while (status && posNext > 0 && posNext < size) {
      this.stream.seekPos(posNext);

      const nextHeaderResult = this.stream.readBlock(RECORD_HEADER_SIZE);
      if (!nextHeaderResult) break;

      posNext = nextHeaderResult.data.readInt32LE(40);
    }

    if (status) {
      this.state = 'modified';
    }

    return { status, compressed: bCompressed };
  }

  /**
   * Import records from a chunk, appending to existing records.
   *
   * Ported from C++ CPQDIF_PC_FlatFile::ImportRecordsFromChunk()
   *
   * @param {Buffer} chunk - The PQDIF data buffer
   * @param {number} size - Size of the buffer
   * @param {boolean} bCompressed - Whether the body is compressed
   * @returns {boolean} True on success
   */
  importRecordsFromChunk(chunk, size, bCompressed) {
    // Clean up old stream/processors
    this._cleanup();

    if (!chunk || size <= 0) return false;

    // Create stream and processors
    this.stream = new ChunkStreamIO();
    this.processorHeader = new NothingProcessor();
    this.processorBody = bCompressed
      ? this._createProcessor(ID_COMP_ALG_ZLIB)
      : new NothingProcessor();

    this.stream.setInput(chunk, size);

    let posNext = 0;
    let status = true;

    // Read through all record headers and bodies
    while (status && posNext < size) {
      this.stream.seekPos(posNext);

      // Read header
      this.stream.connectProcessor(this.processorHeader);
      const headerResult = this.stream.readBlock(RECORD_HEADER_SIZE);
      if (!headerResult) break;

      const headerBuf = headerResult.data;
      const sizeData = headerBuf.readInt32LE(36);
      posNext = headerBuf.readInt32LE(40);

      // Read body
      this.stream.connectProcessor(this.processorBody);
      const bodyResult = this.stream.readBlock(sizeData);
      if (!bodyResult) break;

      if (posNext === 0) break;
    }

    this.state = 'modified';
    return status;
  }

  // -----------------------------------------------------------------------
  //  Write operations
  // -----------------------------------------------------------------------

  /**
   * Write all records to a new file.
   *
   * Ported from C++ CPQDIF_PC_FlatFile::WriteNew()
   *
   * @returns {boolean} True on success
   */
  writeNew() {
    const useChunk = this.chunk !== null;

    // Create new stream for writing
    let writeStream;
    if (useChunk) {
      writeStream = new ChunkStreamIO();
    } else {
      writeStream = new FileStreamIO();
      if (!writeStream.createNew(this.fileName)) {
        return false;
      }
    }

    const procHeader = new NothingProcessor();
    const procBodyFirstRecord = new NothingProcessor();

    // Determine body processor based on compression style
    let procBody;
    switch (this.compressionStyle) {
      case ID_COMP_STYLE_TOTALFILE:
        return false; // Unsupported

      case ID_COMP_STYLE_RECORDLEVEL:
        procBody = this._createProcessor(this.compressionAlgorithm);
        break;

      case ID_COMP_STYLE_NONE:
      default:
        procBody = new NothingProcessor();
        break;
    }

    if (!writeStream || !procHeader || !procBody || !procBodyFirstRecord) {
      return false;
    }

    const countRecords = this.records.length;
    let posNextRecord = 0;

    for (let idxRecord = 0; idxRecord < countRecords; idxRecord++) {
      const precord = this.records[idxRecord];
      if (!precord) break;

      // Set initial header position
      if (precord.headerSetPos) {
        precord.headerSetPos(posNextRecord);
      }

      // Get header sizes
      let sizeHeader = RECORD_HEADER_SIZE;
      let sizeBody = 0;
      if (precord.headerGetSize) {
        const sizes = precord.headerGetSize();
        if (sizes.header > 0) sizeHeader = sizes.header;
        if (sizes.body > 0) sizeBody = sizes.body;
      }

      const posCurrentRecordBody = posNextRecord + sizeHeader;

      // Seek to the body start position (past the header space)
      if (writeStream.seekPos) {
        writeStream.seekPos(posCurrentRecordBody);
      }

      // Write the body
      if (idxRecord === 0) {
        writeStream.connectProcessor(procBodyFirstRecord);
      } else {
        writeStream.connectProcessor(procBody);
      }

      if (precord.writeBodyStream) {
        precord.writeBodyStream(writeStream);
      } else if (precord.writeBody) {
        precord.writeBody(writeStream);
      }

      // Determine where next record starts
      posNextRecord = writeStream.getPos();

      // Update body size
      sizeBody = posNextRecord - posCurrentRecordBody;
      if (precord.headerSetSize) {
        precord.headerSetSize(sizeHeader, sizeBody);
      }

      // Set next record pointer (0 for last record)
      if (idxRecord === countRecords - 1) {
        posNextRecord = 0;
      }
      if (precord.headerSetPosNextRecord) {
        precord.headerSetPosNextRecord(posNextRecord);
      }

      // Save the current position (end of body)
      const posAfterBody = writeStream.getPos();

      // Write the header at the record's position
      writeStream.connectProcessor(procHeader);
      if (precord.WriteHeader) {
        precord.WriteHeader(writeStream);
      }

      // Seek back to the end of body for the next record
      if (writeStream.seekPos) {
        writeStream.seekPos(posAfterBody);
      }
    }

    writeStream.flush();

    // Replace old stream/processors
    this.stream = writeStream;
    this.processorHeader = procHeader;
    this.processorBody = procBody;

    return true;
  }

  /**
   * Write records to a file (equivalent to WriteRecordsToFile).
   *
   * Ported from C++ CPQDIF_PC_FlatFile::WriteRecordsToFile()
   *
   * @returns {boolean} True on success
   */
  writeRecordsToFile() {
    return this.writeNew();
  }

  /**
   * Write to an existing stream (for embedding PQDIF in another format).
   *
   * Ported from C++ CPQDIF_PC_FlatFile::WriteToStream()
   *
   * @param {import('../io/streamIO').StreamIO} pstrm - The output stream
   * @returns {boolean} True on success
   */
  writeToStream(pstrm) {
    if (!pstrm) return false;

    const procHeader = new NothingProcessor();
    const procBodyFirstRecord = new NothingProcessor();

    // Determine body processor
    let procBody;
    switch (this.compressionStyle) {
      case ID_COMP_STYLE_TOTALFILE:
        return false;

      case ID_COMP_STYLE_RECORDLEVEL:
        procBody = this._createProcessor(this.compressionAlgorithm);
        break;

      case ID_COMP_STYLE_NONE:
      default:
        procBody = new NothingProcessor();
        break;
    }

    const countRecords = this.records.length;
    let posNextRecord = 0;

    for (let idxRecord = 0; idxRecord < countRecords; idxRecord++) {
      const precord = this.records[idxRecord];
      if (!precord) break;

      // Ensure body is up-to-date by reading from old stream
      if (this.stream && this.processorBody && precord.ReadBody) {
        this.stream.connectProcessor(this.processorBody);
        precord.ReadBody(this.stream);
      }

      // Set initial header position
      if (precord.headerSetPos) {
        precord.headerSetPos(posNextRecord);
      }

      let sizeHeader = RECORD_HEADER_SIZE;
      let sizeBody = 0;
      if (precord.headerGetSize) {
        const sizes = precord.headerGetSize();
        sizeHeader = sizes.header;
        sizeBody = sizes.body;
      }

      const posCurrentRecordBody = posNextRecord + sizeHeader;

      // Write the body
      if (idxRecord === 0) {
        pstrm.connectProcessor(procBodyFirstRecord);
      } else {
        pstrm.connectProcessor(procBody);
      }

      if (precord.writeBody) {
        precord.writeBody(pstrm);
      }

      posNextRecord = pstrm.getPos();

      // Update header info
      sizeBody = posNextRecord - posCurrentRecordBody;
      if (precord.headerSetSize) {
        precord.headerSetSize(sizeHeader, sizeBody);
      }

      if (idxRecord === countRecords - 1) {
        posNextRecord = 0;
      }
      if (precord.headerSetPosNextRecord) {
        precord.headerSetPosNextRecord(posNextRecord);
      }

      // Write the header
      pstrm.connectProcessor(procHeader);
      if (precord.WriteHeader) {
        precord.WriteHeader(pstrm);
      }
    }

    pstrm.flush();
    return true;
  }

  // -----------------------------------------------------------------------
  //  Get full record body (lazy loading)
  // -----------------------------------------------------------------------

  /**
   * Read the full body for a record at the given index.
   *
   * Ported from C++ CPQDIF_PC_FlatFile::GetRecordFull()
   *
   * @param {number} index - Record index
   * @returns {Object|null} The record with its body loaded, or null
   */
  getRecordFull(index) {
    const prec = this.getRecord(index);
    if (!prec || !this.stream || !this.processorBody) {
      return prec;
    }

    // Connect the body processor and read the body
    this.stream.connectProcessor(this.processorBody);
    if (prec.ReadBody) {
      const status = prec.ReadBody(this.stream);
      if (!status) return null;
    }

    return prec;
  }

  // -----------------------------------------------------------------------
  //  Private helpers
  // -----------------------------------------------------------------------

  /**
   * Clean up stream and processor objects.
   * @private
   */
  _cleanup() {
    if (this.stream) {
      if (this.stream.close) {
        this.stream.close();
      }
      this.stream = null;
    }
    this.processorHeader = null;
    this.processorBody = null;
  }

  /**
   * Create a stream processor for the given compression algorithm.
   * @param {number} algorithm - Compression algorithm ID
   * @returns {import('../io/streamProcessor').StreamProcessor}
   * @private
   */
  _createProcessor(algorithm) {
    switch (algorithm) {
      case ID_COMP_ALG_ZLIB:
        return new ZlibProcessor();
      case ID_COMP_ALG_NONE:
      default:
        return new NothingProcessor();
    }
  }

  /**
   * Parse compression information from the container record body.
   *
   * Looks for tagCompressionStyleID and tagCompressionAlgorithmID
   * in the container's main collection.
   *
   * @param {Buffer} bodyBuf - Record body buffer
   * @param {number} bodySize - Size of the body
   * @returns {boolean} True if compression info was found
   * @private
   */
  _parseContainerCompression(bodyBuf, bodySize) {
    // Try to parse the collection to find compression tags
    // The body starts with a c_collection header
    if (bodySize < 4) return false;

    const count = bodyBuf.readInt32LE(0);
    if (count <= 0) return false;

    let foundStyle = false;
    let foundAlgorithm = false;

    // Walk through collection elements looking for compression tags
    const elemBase = 4; // After the count header
    for (let i = 0; i < count; i++) {
      const elemOffset = elemBase + i * 28;
      if (elemOffset + 28 > bodySize) break;

      // Read the tag (16 bytes)
      const tag = bodyBuf.subarray(elemOffset, elemOffset + 16);

      // Check if this is a compression style tag
      if (tag.equals(tagGuids.tagCompressionStyleID)) {
        const isEmbedded = bodyBuf.readUInt8(elemOffset + 18);
        if (isEmbedded) {
          this.compressionStyle = bodyBuf.readUInt32LE(elemOffset + 20);
        } else {
          const linkOffset = bodyBuf.readInt32LE(elemOffset + 20);
          if (linkOffset > 0 && linkOffset + 4 <= bodySize) {
            this.compressionStyle = bodyBuf.readInt32LE(linkOffset);
          }
        }
        foundStyle = true;
      }

      // Check if this is a compression algorithm tag
      if (tag.equals(tagGuids.tagCompressionAlgorithmID)) {
        const isEmbedded = bodyBuf.readUInt8(elemOffset + 18);
        if (isEmbedded) {
          this.compressionAlgorithm = bodyBuf.readUInt32LE(elemOffset + 20);
        } else {
          const linkOffset = bodyBuf.readInt32LE(elemOffset + 20);
          if (linkOffset > 0 && linkOffset + 4 <= bodySize) {
            this.compressionAlgorithm = bodyBuf.readInt32LE(linkOffset);
          }
        }
        foundAlgorithm = true;
      }

      if (foundStyle && foundAlgorithm) break;
    }

    return foundStyle || foundAlgorithm;
  }
}

module.exports = FlatFileController;
