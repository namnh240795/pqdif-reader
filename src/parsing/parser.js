'use strict';

const { ID_ELEMENT_TYPE_COLLECTION, ID_ELEMENT_TYPE_SCALAR, ID_ELEMENT_TYPE_VECTOR } = require('../constants/elementTypes');
const { PqdifValue } = require('../core/pqdifValue');
const { PqdifInfo } = require('../info/info');

// Size of a c_collection_element structure in bytes:
// 16 (tag GUID) + 1 (typeElement) + 1 (typePhysical) + 1 (isEmbedded) + 1 (reserved) + 8 (data union) = 28
const COLLECTION_ELEMENT_SIZE = 28;

/**
 * PQController - controls the reconstitution of PQDIF element objects
 * from a binary buffer.
 *
 * Ported from ser_cont_el.h / ser_cont_el.cpp
 *
 * This is the callback interface that PQDIFIterator uses to deliver
 * parsed elements (collections, scalars, vectors) as they are encountered.
 */
class PQController {
  /**
   * @param {import('../core/element').Element} parentCollection - The parent collection to add parsed elements to
   */
  constructor(parentCollection) {
    this.parentCollection = parentCollection;
  }

  /**
   * Parse a record body from a binary buffer.
   * Creates a PQDIFIterator and recursively parses the collection tree.
   *
   * @param {Buffer} buffer - The record body buffer
   * @param {number} offset - Starting offset within the buffer (usually 0)
   * @param {number} size - Total size of the record body
   */
  parseRecord(buffer, offset, size) {
    const iterator = new PQDIFIterator(this, buffer, size, offset, this.parentCollection);
    iterator.parseCollection();
  }

  /**
   * Accept a sub-collection element during parsing.
   * Creates a new Collection element, sets its tag, and adds it to the parent.
   *
   * @param {Buffer} tag - 16-byte GUID tag
   * @param {import('../core/element').Element} collection - The new collection element
   * @returns {import('../core/element').Element} The collection (or null on failure)
   */
  acceptCollection(tag, collection) {
    collection.setTag(tag);
    this.parentCollection.add(collection);
    return collection;
  }

  /**
   * Accept a scalar element during parsing.
   * Creates a new Scalar element, decodes the value, and adds it to the parent.
   *
   * @param {Buffer} tag - 16-byte GUID tag
   * @param {number} physicalType - Physical type ID
   * @param {Buffer} dataBuffer - Buffer containing the scalar data value
   * @returns {import('../core/element').Element|null} The scalar element or null
   */
  acceptScalar(tag, physicalType, dataBuffer) {
    const factory = require('../core/factory');
    const pel = factory.newElement(ID_ELEMENT_TYPE_SCALAR);
    if (!pel) return null;

    pel.setTag(tag);

    // Decode the PQDIFValue from the data buffer
    const value = PqdifValue.readFromBuffer(dataBuffer, 0, physicalType);
    if (pel.setValue(physicalType, value)) {
      this.parentCollection.add(pel);
      return pel;
    }

    return null;
  }

  /**
   * Accept a vector element during parsing.
   * Creates a new Vector element, copies the raw data, and adds it to the parent.
   *
   * @param {Buffer} tag - 16-byte GUID tag
   * @param {number} physicalType - Physical type ID
   * @param {number} count - Number of elements in the vector
   * @param {Buffer} dataBuffer - Buffer containing the vector data
   * @returns {import('../core/element').Element|null} The vector element or null
   */
  acceptVector(tag, physicalType, count, dataBuffer) {
    const factory = require('../core/factory');

    // Validate count (matching C++: count must be > 0 and < 16MB)
    const sizeValue = PqdifInfo.getTypeSize(physicalType);
    const totalSize = sizeValue * count;
    if (totalSize <= 0 || count <= 0 || count >= 16 * 1024 * 1024) {
      return null;
    }

    const pel = factory.newElement(ID_ELEMENT_TYPE_VECTOR);
    if (!pel) return null;

    pel.setTag(tag);
    pel.setPhysicalType(physicalType);

    if (pel.setCount(count)) {
      // Copy raw data into the vector's internal buffer
      const destRaw = pel.getRawData();
      dataBuffer.copy(destRaw, 0, 0, count * sizeValue);
      this.parentCollection.add(pel);
      return pel;
    }

    return null;
  }

  /**
   * Decode a PQDIFValue from a raw buffer.
   * Static helper matching C++ PQController::decodeValue().
   *
   * @param {number} typePhysical - Physical type ID
   * @param {Buffer} pdata - Buffer containing the value data
   * @returns {PqdifValue|null} Decoded value or null on failure
   */
  static decodeValue(typePhysical, pdata) {
    const iSize = PqdifInfo.getTypeSize(typePhysical);
    if (iSize > 0 && pdata && pdata.length >= iSize) {
      return PqdifValue.readFromBuffer(pdata, 0, typePhysical);
    }
    return null;
  }
}

/**
 * PQDIFIterator - iterates through collection elements in a binary buffer
 * to reconstitute PQDIF element objects.
 *
 * Ported from ser_iter_el.h / ser_iter_el.cpp
 *
 * The iterator walks through the c_collection_element array within a record
 * body buffer, handling three element types:
 * - COLLECTION: recursively parses sub-collections via link offset
 * - SCALAR: reads embedded values or follows link to external data
 * - VECTOR: follows link to c_vector header + data array
 */
class PQDIFIterator {
  /**
   * @param {PQController} controller - The controller to deliver parsed elements to
   * @param {Buffer} buffer - The record body buffer
   * @param {number} size - Total size of the record body
   * @param {number} pos - Starting position within the buffer
   * @param {import('../core/element').Element} collection - The collection being parsed
   */
  constructor(controller, buffer, size, pos, collection) {
    this.controller = controller;
    this.buffer = buffer;
    this.pos = pos;
    this.size = size;
    this.collection = collection;
  }

  /**
   * Parse the collection at the current position.
   *
   * Binary format:
   *   [c_collection header: 4 bytes (count)]
   *   [c_collection_element array: count * 28 bytes]
   *
   * Each c_collection_element (28 bytes):
   *   [16 bytes: tag GUID]
   *   [1 byte:   typeElement (1=Collection, 2=Scalar, 3=Vector)]
   *   [1 byte:   typePhysical (physical type of value)]
   *   [1 byte:   isEmbedded (0=use link, 1=value in embedded field)]
   *   [1 byte:   reserved]
   *   [8 bytes:  data union - either link.linkElement+link.sizeElement or valueEmbedded]
   *
   * Link offsets are RELATIVE to the start of the record body (buffer[0]).
   *
   * @returns {boolean} True if at least one element was accepted
   */
  parseCollection() {
    let accepted = false;

    // Read the collection header (4-byte count)
    if (this.pos + 4 > this.size) {
      return false;
    }
    const count = this.buffer.readInt32LE(this.pos);
    this.pos += 4;

    // Validate position
    if (this.pos > this.size || (this.pos === this.size && count > 0)) {
      return false;
    }

    // Iterate through each collection element (28 bytes each)
    for (let idx = 0; idx < count; idx++) {
      const elemOffset = this.pos + idx * COLLECTION_ELEMENT_SIZE;

      // Bounds check
      if (elemOffset + COLLECTION_ELEMENT_SIZE > this.size) {
        break;
      }

      // Parse the 28-byte collection element
      const tag = Buffer.from(this.buffer.subarray(elemOffset, elemOffset + 16));
      const typeElement = this.buffer.readInt8(elemOffset + 16);
      const typePhysical = this.buffer.readInt8(elemOffset + 17);
      const isEmbedded = this.buffer.readUInt8(elemOffset + 18);
      // reserved at offset+19 (1 byte, ignored)

      // Read the 8-byte data union
      // For non-embedded: link.linkElement (4 bytes LE) + link.sizeElement (4 bytes LE)
      // For embedded: valueEmbedded (8 bytes)
      const dataUnionOffset = elemOffset + 20;

      switch (typeElement) {
        case ID_ELEMENT_TYPE_COLLECTION: {
          // Create a new collection element
          const factory = require('../core/factory');
          const newCollection = factory.newElement(ID_ELEMENT_TYPE_COLLECTION);
          if (!newCollection) break;

          // Notify the controller (which sets the tag and adds to parent)
          const acceptedColl = this.controller.acceptCollection(tag, newCollection);
          if (!acceptedColl) break;

          accepted = true;

          if (!isEmbedded) {
            // Follow the link to the sub-collection
            const linkElement = this.buffer.readInt32LE(dataUnionOffset);
            // Validate the link position
            if (linkElement <= 0 || linkElement >= this.size) {
              break;
            }

            // Create a sub-controller targeting the new collection
            const subController = new PQController(newCollection);
            // Recursively parse the sub-collection
            const subIterator = new PQDIFIterator(
              subController,
              this.buffer,
              this.size,
              linkElement,
              newCollection
            );
            subIterator.parseCollection();
          }
          // Note: embedded collections are not expected per the PQDIF spec
          break;
        }

        case ID_ELEMENT_TYPE_SCALAR: {
          let dataPtr;

          if (isEmbedded) {
            // The scalar value is embedded directly in the 8-byte data union
            dataPtr = this.buffer.subarray(dataUnionOffset, dataUnionOffset + 8);
          } else {
            // Follow the link to the external data
            const linkElement = this.buffer.readInt32LE(dataUnionOffset);
            // Validate the link position
            if (linkElement < 0 || linkElement >= this.size) {
              break;
            }
            dataPtr = this.buffer.subarray(linkElement);
          }

          // Accept the scalar via the controller
          const result = this.controller.acceptScalar(tag, typePhysical, dataPtr);
          if (result) {
            accepted = true;
          }
          break;
        }

        case ID_ELEMENT_TYPE_VECTOR: {
          // Follow the link to the vector header
          const linkElement = this.buffer.readInt32LE(dataUnionOffset);
          // Validate the link position
          if (linkElement <= 0 || linkElement >= this.size) {
            break;
          }

          // Read the c_vector header (4-byte count)
          const vectorPos = linkElement;
          if (vectorPos + 4 > this.size) {
            break;
          }
          const vectorCount = this.buffer.readInt32LE(vectorPos);

          // Data starts right after the c_vector header
          const dataStart = vectorPos + 4;
          if (vectorCount > 0) {
            if (dataStart > this.size) {
              break;
            }
          }

          // Accept the vector via the controller
          const vecResult = this.controller.acceptVector(
            tag,
            typePhysical,
            vectorCount,
            this.buffer.subarray(dataStart)
          );
          if (vecResult) {
            accepted = true;
          }
          break;
        }

        default:
          // Unknown element type, skip
          break;
      }
    }

    return accepted;
  }
}

module.exports = { PQController, PQDIFIterator };
