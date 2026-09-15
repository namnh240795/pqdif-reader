'use strict';

const { PqdifInfo } = require('../info/info');
const { ID_ELEMENT_TYPE_COLLECTION, ID_ELEMENT_TYPE_SCALAR, ID_ELEMENT_TYPE_VECTOR } = require('../constants/elementTypes');

// Size of a c_collection_element structure in bytes
const COLLECTION_ELEMENT_SIZE = 28;

/**
 * PQAlloc - a simple allocator for building PQDIF binary data.
 *
 * Ported from ser_alloc.h / ser_alloc.cpp
 *
 * This class manages a byte buffer that grows as PQDIF elements are added.
 * It provides methods to allocate space for collections, scalars, and vectors,
 * and to write the final buffer to a file or stream.
 *
 * All offsets returned by allocate() are ABSOLUTE (idxOffset + buffer position),
 * matching the C++ behavior where offsets represent positions in the output file.
 */
class Serializer {
  constructor() {
    /** @type {Buffer} The data buffer */
    this.data = Buffer.alloc(0);

    /** @type {number} Offset added to all returned indices (for file positioning) */
    this.idxOffset = 0;

    /** @type {number} Current write position within the buffer */
    this.pos = 0;
  }

  /**
   * Reinitialize the allocator, clearing all data.
   * @param {number} [offset=0] - Base offset for the new allocation
   */
  reinitialize(offset = 0) {
    this.data = Buffer.alloc(32 * 1024); // 32KB initial size, matching C++
    this.pos = 0;
    this.idxOffset = offset;
  }

  /**
   * Ensure the buffer has room for additional bytes.
   * @param {number} additionalBytes - Number of bytes needed
   */
  ensureCapacity(additionalBytes) {
    const needed = this.pos + additionalBytes;
    if (needed > this.data.length) {
      const newSize = Math.max(needed, this.data.length * 2);
      const newBuf = Buffer.alloc(newSize);
      this.data.copy(newBuf, 0, 0, this.pos);
      this.data = newBuf;
    }
  }

  /**
   * Allocate a block of memory in the buffer.
   * Returns the absolute offset (pos + idxOffset) of the allocated block.
   *
   * @param {number} size - Number of bytes to allocate
   * @returns {number} Absolute offset of the allocated block, or -1 on failure
   */
  allocate(size) {
    const idx = this.pos + this.idxOffset;

    try {
      this.ensureCapacity(size);
      // Zero-fill the newly allocated area
      this.data.fill(0, this.pos, this.pos + size);
      this.pos += size;
    } catch (e) {
      return -1;
    }

    return idx;
  }

  /**
   * Convert an absolute offset to a pointer (Buffer slice) into the data buffer.
   * @param {number} idx - Absolute offset
   * @returns {Buffer} Slice of the data buffer at the given offset
   */
  at(idx) {
    const localPos = idx - this.idxOffset;
    return this.data.subarray(localPos);
  }

  // -----------------------------------------------------------------------
  //  Collection operations
  // -----------------------------------------------------------------------

  /**
   * Allocate space for a collection (header + element array).
   *
   * Size = sizeof(c_collection) + count * sizeof(c_collection_element), padded to 4 bytes.
   *
   * @param {number} count - Number of elements in the collection
   * @returns {{ offset: number, size: number }|null} Allocation info or null on failure
   */
  addCollection(count) {
    // Calculate size: 4 (count header) + count * 28 (element array)
    let size = 4 + count * COLLECTION_ELEMENT_SIZE;
    size = PqdifInfo.getTypeSize ? padTo4Bytes(size) : ((size + 3) & ~3);

    const offset = this.allocate(size);
    if (offset < 0) return null;

    // Write the collection count header
    const collBuf = this.at(offset);
    collBuf.writeInt32LE(count, 0);

    return { offset, size };
  }

  /**
   * Create a sub-collection and link it to a collection element.
   *
   * @param {number} count - Number of elements in the sub-collection
   * @param {Buffer} tag - 16-byte GUID tag
   * @param {number} idxCE - Absolute offset of the collection element to link from
   * @returns {number} Absolute offset of the new collection, or -1 on failure
   */
  addCollectionValue(count, tag, idxCE) {
    const result = this.addCollection(count);
    if (!result) return -1;

    // Set up the collection element that references this collection
    const pce = this.at(idxCE);

    // Write tag (16 bytes)
    tag.copy(pce, 0, 0, 16);

    // Write typeElement, typePhysical, isEmbedded, reserved
    pce.writeInt8(ID_ELEMENT_TYPE_COLLECTION, 16); // typeElement
    pce.writeInt8(0, 17);                            // typePhysical (0 for collections)
    pce.writeUInt8(0, 18);                           // isEmbedded = FALSE
    pce.writeInt8(0, 19);                            // reserved

    // Write link (offset + size) in the 8-byte data union
    pce.writeInt32LE(result.offset, 20);  // link.linkElement
    pce.writeInt32LE(result.size, 24);    // link.sizeElement

    return result.offset;
  }

  // -----------------------------------------------------------------------
  //  Scalar operations
  // -----------------------------------------------------------------------

  /**
   * Add a scalar value to a collection element.
   *
   * If the value fits in 8 bytes, it is embedded directly in the
   * collection element's data union. Otherwise, it is allocated
   * separately and referenced via a link.
   *
   * @param {number} typePhysical - Physical type ID
   * @param {PqdifValue} value - The value to write
   * @param {Buffer} tag - 16-byte GUID tag
   * @param {number} idxCE - Absolute offset of the collection element to fill
   * @returns {boolean} True on success
   */
  addScalarValue(typePhysical, value, tag, idxCE) {
    // Get the pce pointer (refreshed after any allocate calls)
    let pce = this.at(idxCE);

    // Write element header
    tag.copy(pce, 0, 0, 16);
    pce.writeInt8(ID_ELEMENT_TYPE_SCALAR, 16);  // typeElement
    pce.writeInt8(typePhysical, 17);              // typePhysical
    pce.writeUInt8(0, 18);                        // isEmbedded = FALSE (default)
    pce.writeInt8(0, 19);                         // reserved

    // Compute the size of the data value
    const typeSize = PqdifInfo.getTypeSize(typePhysical);

    if (typeSize <= 8) {
      // Embed the value directly in the collection element's data union (8 bytes)
      pce.writeUInt8(1, 18); // isEmbedded = TRUE

      // Write the value into the 8-byte data union at offset 20
      const embeddedBuf = pce.subarray(20, 28);
      embeddedBuf.fill(0);
      value.writeToBuffer(embeddedBuf, 0);
    } else {
      // Allocate separate storage (padded to 4 bytes)
      const allocSize = padTo4Bytes(typeSize);
      const dataOffset = this.allocate(allocSize);
      if (dataOffset < 0) return false;

      // Refresh pce pointer after allocate (buffer may have been reallocated)
      pce = this.at(idxCE);

      // Write link info
      pce.writeUInt8(0, 18);                       // isEmbedded = FALSE
      pce.writeInt32LE(dataOffset, 20);            // link.linkElement
      pce.writeInt32LE(allocSize, 24);             // link.sizeElement

      // Write the value data
      const dataPtr = this.at(dataOffset);
      value.writeToBuffer(dataPtr, 0);
    }

    return true;
  }

  // -----------------------------------------------------------------------
  //  Vector operations
  // -----------------------------------------------------------------------

  /**
   * Add a vector value to a collection element.
   *
   * Allocates space for the c_vector header (4 bytes) + data array,
   * copies the values, and links from the collection element.
   *
   * @param {number} typePhysical - Physical type ID
   * @param {number} count - Number of elements in the vector
   * @param {Buffer} values - Raw buffer containing the vector data
   * @param {Buffer} tag - 16-byte GUID tag
   * @param {number} idxCE - Absolute offset of the collection element to fill
   * @returns {boolean} True on success
   */
  addVectorValue(typePhysical, count, values, tag, idxCE) {
    // Calculate size: c_vector header (4 bytes) + count * typeSize
    const sizeValue = PqdifInfo.getTypeSize(typePhysical);
    let size = 4 + (count * sizeValue);
    size = padTo4Bytes(size);

    // Allocate the vector
    const idx = this.allocate(size);
    if (idx < 0) return false;

    // Set up the collection element header
    let pce = this.at(idxCE);
    tag.copy(pce, 0, 0, 16);
    pce.writeInt8(ID_ELEMENT_TYPE_VECTOR, 16);  // typeElement
    pce.writeInt8(typePhysical, 17);              // typePhysical
    pce.writeUInt8(0, 18);                        // isEmbedded = FALSE
    pce.writeInt8(0, 19);                         // reserved
    pce.writeInt32LE(idx, 20);                    // link.linkElement
    pce.writeInt32LE(size, 24);                   // link.sizeElement

    // Write the c_vector header (count)
    const vectorBuf = this.at(idx);
    vectorBuf.writeInt32LE(count, 0);

    // Copy the vector data after the header
    if (count > 0 && values && values.length > 0) {
      const dataSize = count * sizeValue;
      values.copy(vectorBuf, 4, 0, dataSize);
    }

    return true;
  }

  // -----------------------------------------------------------------------
  //  BufferUpCollection - serialize a full collection tree
  // -----------------------------------------------------------------------

  /**
   * Serialize an entire collection tree into the internal buffer.
   * This is a high-level method that recursively serializes a collection
   * and all its child elements.
   *
   * @param {import('../core/element').Element} collection - The root collection to serialize
   * @returns {{ offset: number, size: number }|null} Allocation info or null on failure
   */
  bufferUpCollection(collection) {
    if (!collection || typeof collection.getChildren !== 'function') {
      return null;
    }

    const children = collection.getChildren();
    const count = children.length;

    // Allocate the collection header + element array
    const collResult = this.addCollection(count);
    if (!collResult) return null;

    // Fill in each collection element
    for (let i = 0; i < count; i++) {
      const child = children[i];
      const childTag = child.getTag();
      const childType = child.getElementType();
      const elemOffset = collResult.offset + 4 + (i * COLLECTION_ELEMENT_SIZE);

      switch (childType) {
        case ID_ELEMENT_TYPE_COLLECTION: {
          // Recursively serialize the sub-collection
          const subResult = this.bufferUpCollection(child);
          if (subResult) {
            const pce = this.at(elemOffset);
            childTag.copy(pce, 0, 0, 16);
            pce.writeInt8(ID_ELEMENT_TYPE_COLLECTION, 16);
            pce.writeInt8(0, 17);
            pce.writeUInt8(0, 18);
            pce.writeInt8(0, 19);
            pce.writeInt32LE(subResult.offset, 20);
            pce.writeInt32LE(subResult.size, 24);
          }
          break;
        }

        case ID_ELEMENT_TYPE_SCALAR: {
          const value = child.getValue();
          if (value) {
            this.addScalarValue(child.getPhysicalType(), value, childTag, elemOffset);
          }
          break;
        }

        case ID_ELEMENT_TYPE_VECTOR: {
          const rawData = child.getRawData();
          if (rawData) {
            this.addVectorValue(
              child.getPhysicalType(),
              child.getCount(),
              rawData,
              childTag,
              elemOffset
            );
          }
          break;
        }
      }
    }

    return collResult;
  }

  // -----------------------------------------------------------------------
  //  Output methods
  // -----------------------------------------------------------------------

  /**
   * Write the entire buffer to a file.
   * @param {string} filePath - Path to write to
   * @returns {number} Number of bytes written
   */
  writeToFile(filePath) {
    const fs = require('fs');
    const fd = fs.openSync(filePath, 'w');
    try {
      fs.writeSync(fd, this.data, 0, this.pos, null);
    } finally {
      fs.closeSync(fd);
    }
    return this.idxOffset + this.pos;
  }

  /**
   * Write the buffer to a StreamIO object.
   * Matches C++ PQAlloc::WriteListToStream behavior.
   *
   * @param {import('../io/streamIO').StreamIO} stream - The stream to write to
   * @returns {number} Number of bytes actually written
   */
  writeListToStream(stream) {
    stream.beginBlock();
    stream.appendBlock(this.data, this.pos);
    stream.writeBlock();
    return stream.posWrite;
  }

  /**
   * Get the current data buffer (trimmed to actual size).
   * @returns {Buffer}
   */
  getBuffer() {
    return this.data.subarray(0, this.pos);
  }

  /**
   * Get the current buffer size.
   * @returns {number}
   */
  getSize() {
    return this.pos;
  }
}

// -----------------------------------------------------------------------
//  Helper functions
// -----------------------------------------------------------------------

/**
 * Pad a size to the next 4-byte boundary.
 * Matches C++ CPQDIF_Info::padSizeTo4Bytes().
 * @param {number} size
 * @returns {number}
 */
function padTo4Bytes(size) {
  return (size + 3) & ~3;
}

module.exports = Serializer;
module.exports.Serializer = Serializer;
module.exports.padTo4Bytes = padTo4Bytes;
