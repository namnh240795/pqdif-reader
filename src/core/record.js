'use strict';

const { Collection } = require('./collection');
const { Scalar } = require('./scalar');
const { Vector } = require('./vector');
const { PqdifInfo, guidEquals } = require('../info/info');
const { RECORD_HEADER_SIZE, GUID_RECORD_SIGNATURE_PQDIF } = require('../constants/physicalTypes');

/**
 * CPQDIFRecord base class.
 * Ported from rec_base.h/rec_base.cpp and rec_general.h/rec_general.cpp
 */
class Record {
  constructor() {
    this.posThisRecord = 0;
    this.headerRecord = {
      guidRecordSignature: Buffer.from(GUID_RECORD_SIGNATURE_PQDIF),
      tagRecordType: null,       // 16-byte Buffer
      sizeHeader: RECORD_HEADER_SIZE,
      sizeData: 0,
      linkNextRecord: 0,         // Absolute file offset
      checksum: 0,
      reserved: Buffer.alloc(16),
    };
    this.mainCollection = null;  // Collection
    this.changed = false;
  }

  // ============================================================
  // Header accessors
  // ============================================================

  getHeaderPos() { return this.posThisRecord; }
  setHeaderPos(pos) { this.posThisRecord = pos; }

  getHeaderTag() { return this.headerRecord.tagRecordType; }
  setHeaderTag(tag) { this.headerRecord.tagRecordType = tag; }

  getHeaderSize() { return this.headerRecord.sizeData; }
  setHeaderSize(size) { this.headerRecord.sizeData = size; }

  getHeaderChecksum() { return this.headerRecord.checksum; }
  setHeaderChecksum(cs) { this.headerRecord.checksum = cs; }

  getHeaderPosNextRecord() { return this.headerRecord.linkNextRecord; }
  setHeaderPosNextRecord(pos) { this.headerRecord.linkNextRecord = pos; }

  getMainCollection() { return this.mainCollection; }
  setMainCollection(coll) {
    if (this.mainCollection) {
      // Don't delete - just dereference
    }
    this.mainCollection = coll;
    if (coll) {
      coll.setRecord(this);
    }
  }

  getChanged() { return this.changed; }
  setChanged(changed) { this.changed = changed; }

  // ============================================================
  // Header I/O (64 bytes)
  // ============================================================

  /**
   * Read record header from buffer.
   * @param {Buffer} buffer
   * @param {number} offset
   * @returns {boolean}
   */
  readHeader(buffer, offset) {
    if (buffer.length - offset < RECORD_HEADER_SIZE) return false;

    this.posThisRecord = offset;

    // guidRecordSignature: bytes 0-15
    this.headerRecord.guidRecordSignature = Buffer.from(buffer.subarray(offset, offset + 16));

    // tagRecordType: bytes 16-31
    this.headerRecord.tagRecordType = Buffer.from(buffer.subarray(offset + 16, offset + 32));

    // sizeHeader: bytes 32-35 (LE uint32)
    this.headerRecord.sizeHeader = buffer.readUInt32LE(offset + 32);

    // sizeData: bytes 36-39 (LE uint32)
    this.headerRecord.sizeData = buffer.readUInt32LE(offset + 36);

    // linkNextRecord: bytes 40-43 (LE int32)
    this.headerRecord.linkNextRecord = buffer.readInt32LE(offset + 40);

    // checksum: bytes 44-47 (LE uint32)
    this.headerRecord.checksum = buffer.readUInt32LE(offset + 44);

    // reserved: bytes 48-63
    this.headerRecord.reserved = Buffer.from(buffer.subarray(offset + 48, offset + 64));

    // Validate signature
    if (!guidEquals(this.headerRecord.guidRecordSignature, GUID_RECORD_SIGNATURE_PQDIF)) {
      return false;
    }

    return true;
  }

  /**
   * Write record header to buffer.
   * @param {Buffer} buffer
   * @param {number} offset
   * @returns {boolean}
   */
  writeHeader(buffer, offset) {
    if (buffer.length - offset < RECORD_HEADER_SIZE) return false;

    // guidRecordSignature: bytes 0-15
    this.headerRecord.guidRecordSignature.copy(buffer, offset);

    // tagRecordType: bytes 16-31
    if (this.headerRecord.tagRecordType) {
      this.headerRecord.tagRecordType.copy(buffer, offset + 16);
    }

    // sizeHeader: bytes 32-35
    buffer.writeUInt32LE(this.headerRecord.sizeHeader, offset + 32);

    // sizeData: bytes 36-39
    buffer.writeUInt32LE(this.headerRecord.sizeData, offset + 36);

    // linkNextRecord: bytes 40-43
    buffer.writeInt32LE(this.headerRecord.linkNextRecord, offset + 40);

    // checksum: bytes 44-47
    buffer.writeUInt32LE(this.headerRecord.checksum, offset + 44);

    // reserved: bytes 48-63
    this.headerRecord.reserved.copy(buffer, offset + 48);

    return true;
  }

  /**
   * Read record body - parse collection from buffer.
   * @param {Buffer} buffer
   * @param {number} offset
   * @param {number} size
   * @returns {boolean}
   */
  readBody(buffer, offset, size) {
    if (this.mainCollection) {
      return true; // Already read
    }

    // Create top-level collection
    const factory = require('./factory');
    this.mainCollection = factory.newElement(1); // ID_ELEMENT_TYPE_COLLECTION
    if (!this.mainCollection) return false;

    // Link collection to record
    this.mainCollection.setRecord(this);

    // Attach record tag to main collection
    this.mainCollection.setTag(this.headerRecord.tagRecordType);

    this.changed = false;
    return true;
  }

  /**
   * Write record body - serialize collection to buffer.
   * @param {Buffer} buffer
   * @param {number} offset
   * @returns {boolean}
   */
  writeBody(buffer, offset) {
    // Override in subclasses or use serialization module
    return true;
  }

  // ============================================================
  // Stream-based write methods used by FlatFileController.writeNew
  // ============================================================

  /**
   * Set the position of this record in the output file.
   * @param {number} pos
   */
  headerSetPos(pos) {
    this.posThisRecord = pos;
  }

  /**
   * Get header and body sizes.
   * @returns {{ header: number, body: number }}
   */
  headerGetSize() {
    return { header: this.headerRecord.sizeHeader, body: this.headerRecord.sizeData };
  }

  /**
   * Set header and body sizes.
   * @param {number} headerSize
   * @param {number} bodySize
   */
  headerSetSize(headerSize, bodySize) {
    this.headerRecord.sizeHeader = headerSize;
    this.headerRecord.sizeData = bodySize;
  }

  /**
   * Set the position of the next record.
   * @param {number} pos
   */
  headerSetPosNextRecord(pos) {
    this.headerRecord.linkNextRecord = pos;
  }

  /**
   * Write the record header to a StreamIO.
   * Seeks to the record position before writing.
   * @param {import('../io/streamIO').StreamIO} stream
   * @returns {boolean}
   */
  WriteHeader(stream) {
    const { RECORD_HEADER_SIZE } = require('../constants/physicalTypes');
    const buf = Buffer.alloc(RECORD_HEADER_SIZE);

    // guidRecordSignature: bytes 0-15
    this.headerRecord.guidRecordSignature.copy(buf, 0);

    // tagRecordType: bytes 16-31
    if (this.headerRecord.tagRecordType) {
      this.headerRecord.tagRecordType.copy(buf, 16);
    }

    // sizeHeader: bytes 32-35
    buf.writeUInt32LE(this.headerRecord.sizeHeader, 32);

    // sizeData: bytes 36-39
    buf.writeUInt32LE(this.headerRecord.sizeData, 36);

    // linkNextRecord: bytes 40-43
    buf.writeInt32LE(this.headerRecord.linkNextRecord, 40);

    // checksum: bytes 44-47
    buf.writeUInt32LE(this.headerRecord.checksum, 44);

    // reserved: bytes 48-63
    this.headerRecord.reserved.copy(buf, 48);

    // Seek to the header position before writing
    if (stream.seekPos && typeof stream.seekPos === 'function') {
      stream.seekPos(this.posThisRecord);
    }

    // Write to stream
    stream.beginBlock();
    stream.appendBlock(buf, RECORD_HEADER_SIZE);
    stream.writeBlock();

    return true;
  }

  /**
   * Write the record body (serialized collection) to a StreamIO.
   * Uses the Serializer to convert the mainCollection tree to binary.
   * @param {import('../io/streamIO').StreamIO} stream
   * @returns {boolean}
   */
  writeBodyStream(stream) {
    if (!this.mainCollection) return true; // Empty record

    const Serializer = require('../serialization/serializer');
    const serializer = new Serializer();
    serializer.reinitialize(stream.getPos());

    const result = serializer.bufferUpCollection(this.mainCollection);
    if (!result) return false;

    // Write serialized data to stream
    stream.beginBlock();
    stream.appendBlock(serializer.getBuffer(), serializer.getSize());
    stream.writeBlock();

    return true;
  }

  // ============================================================
  // Convenience methods for main collection
  // ============================================================

  setTimeInMainCollection(tag, timeValue) {
    if (!this.mainCollection) return false;
    const factory = require('./factory');
    let psc = this.mainCollection.getElementByTag(tag, 2);
    if (!psc) {
      psc = factory.newElement(2); // Scalar
      if (psc) {
        psc.setTag(tag);
        psc.setPhysicalType(50); // TIMESTAMPPQDIF
        this.mainCollection.add(psc);
      }
    }
    if (psc) {
      return psc.setValueTimeStamp(timeValue);
    }
    return false;
  }

  setREAL8InMainCollection(tag, dVal) {
    if (!this.mainCollection) return false;
    const factory = require('./factory');
    let psc = this.mainCollection.getElementByTag(tag, 2);
    if (!psc) {
      psc = factory.newElement(2);
      if (psc) {
        psc.setTag(tag);
        psc.setPhysicalType(41); // REAL8
        this.mainCollection.add(psc);
      }
    }
    if (psc) {
      return psc.setValueREAL8(dVal);
    }
    return false;
  }

  setBOOL4InMainCollection(tag, bVal) {
    if (!this.mainCollection) return false;
    const factory = require('./factory');
    let psc = this.mainCollection.getElementByTag(tag, 2);
    if (!psc) {
      psc = factory.newElement(2);
      if (psc) {
        psc.setTag(tag);
        psc.setPhysicalType(3); // BOOLEAN4
        this.mainCollection.add(psc);
      }
    }
    if (psc) {
      return psc.setValueBOOL4(bVal);
    }
    return false;
  }

  getTimeInMainCollection(tag, outTime) {
    if (!this.mainCollection) return false;
    const psc = this.mainCollection.getElementByTag(tag, 2);
    if (psc) {
      const out = { value: null };
      const status = psc.GetValueTimeStamp(out);
      if (status) {
        outTime.value = out.value;
      }
      return status;
    }
    return false;
  }

  GetTimeInMainCollection(tag, out) {
    const result = { value: { day: 0, sec: 0.0 } };
    const status = this.getTimeInMainCollection(tag, result);
    if (out && typeof out === 'object') out.value = result.value;
    return status;
  }

  getREAL8InMainCollection(tag, outVal) {
    if (!this.mainCollection) return false;
    const psc = this.mainCollection.getElementByTag(tag, 2);
    if (psc) {
      const out = { value: 0.0 };
      const status = psc.GetValueREAL8(out);
      if (status) outVal.value = out.value;
      return status;
    }
    return false;
  }

  GetREAL8InMainCollection(tag, out) {
    const result = { value: 0.0 };
    const status = this.getREAL8InMainCollection(tag, result);
    if (out && typeof out === 'object') out.value = result.value;
    return status;
  }

  getBOOL4InMainCollection(tag, outVal) {
    if (!this.mainCollection) return false;
    const psc = this.mainCollection.getElementByTag(tag, 2);
    if (psc) {
      const out = { value: false };
      const status = psc.GetValueBOOL4(out);
      if (status) outVal.value = out.value;
      return status;
    }
    return false;
  }

  GetBOOL4InMainCollection(tag, out) {
    const result = { value: false };
    const status = this.getBOOL4InMainCollection(tag, result);
    if (out && typeof out === 'object') out.value = result.value;
    return status;
  }

  // ============================================================
  // Static helper methods (ported from CPQDIFRecord)
  // ============================================================

  static findCollectionInCollection(pcoll, tag) {
    if (!pcoll) return null;
    const pel = pcoll.getElementByTag(tag, 1); // ID_ELEMENT_TYPE_COLLECTION
    if (pel && pel.getElementType() === 1) return pel;
    return null;
  }

  static findScalarInCollection(pcoll, tag) {
    if (!pcoll) return null;
    const pel = pcoll.getElementByTag(tag, 2); // ID_ELEMENT_TYPE_SCALAR
    if (pel && pel.getElementType() === 2) return pel;
    return null;
  }

  static findVectorInCollection(pcoll, tag) {
    if (!pcoll) return null;
    const pel = pcoll.getElementByTag(tag, 3); // ID_ELEMENT_TYPE_VECTOR
    if (pel && pel.getElementType() === 3) return pel;
    return null;
  }

  static getScalarValueInCollection(pcoll, tag, typePhysical, outValue) {
    if (!pcoll) return false;
    const psc = pcoll.getElementByTag(tag, 2);
    if (psc && psc.getElementType() === 2) {
      const out = { typePhysical: 0, value: null };
      const status = psc.getValue(out);
      if (status && out.typePhysical === typePhysical) {
        outValue.value = out.value;
        return true;
      }
    }
    return false;
  }

  static findOrCreateScalarInCollection(pcoll, tag, typePhysical) {
    if (!pcoll) return null;
    let psc = pcoll.getElementByTag(tag, 2);
    if (!psc) {
      const factory = require('./factory');
      psc = factory.newElement(2); // Scalar
      if (psc) {
        psc.setTag(tag);
        psc.setPhysicalType(typePhysical);
        pcoll.add(psc);
      }
    }
    return psc;
  }

  static findOrCreateVectorInCollection(pcoll, tag, typePhysical) {
    if (!pcoll) return null;
    let pvect = pcoll.getElementByTag(tag, 3);
    if (!pvect) {
      const factory = require('./factory');
      pvect = factory.newElement(3); // Vector
      if (pvect) {
        pvect.setTag(tag);
        pvect.setPhysicalType(typePhysical);
        pcoll.add(pvect);
      }
    }
    return pvect;
  }
}

module.exports = { Record };
