'use strict';

const { Element } = require('./element');
const { Scalar } = require('./scalar');
const { Vector } = require('./vector');
const { PqdifValue } = require('./pqdifValue');
const { PqdifInfo, guidEquals } = require('../info/info');
const {
  ID_ELEMENT_TYPE_COLLECTION, ID_ELEMENT_TYPE_SCALAR, ID_ELEMENT_TYPE_VECTOR,
} = require('../constants/elementTypes');
const {
  ID_PHYS_TYPE_UNS_INTEGER4, ID_PHYS_TYPE_INTEGER4, ID_PHYS_TYPE_REAL4, ID_PHYS_TYPE_REAL8,
  ID_PHYS_TYPE_BOOLEAN4, ID_PHYS_TYPE_CHAR1, ID_PHYS_TYPE_GUID, ID_PHYS_TYPE_TIMESTAMPPQDIF,
  ID_PHYS_TYPE_COMPLEX8, ID_PHYS_TYPE_COMPLEX16,
  ID_PHYS_TYPE_UNS_INTEGER2, ID_PHYS_TYPE_INTEGER2,
} = require('../constants/physicalTypes');

/**
 * CPQDIF_E_Collection - sorted array of child elements.
 * Ported from el_coll.h/el_coll.cpp
 */
class Collection extends Element {
  constructor() {
    super();
    this.elements = []; // Sorted by tag GUID
  }

  getElementType() {
    return 1; // ID_ELEMENT_TYPE_COLLECTION
  }

  /**
   * Override SetRecord to propagate to children.
   * @param {Object|null} record
   */
  setRecord(record) {
    super.setRecord(record);
    for (const el of this.elements) {
      if (el) {
        el.setRecord(record);
      }
    }
  }

  /**
   * Get count of elements.
   * @returns {number}
   */
  getCount() {
    return this.elements.length;
  }

  /**
   * Get element at index.
   * @param {number} index
   * @returns {Element|null}
   */
  getElement(index) {
    if (index >= 0 && index < this.elements.length) {
      return this.elements[index];
    }
    return null;
  }

  /**
   * Get all child elements.
   * @returns {Element[]}
   */
  getChildren() {
    return this.elements;
  }

  /**
   * Find element by tag GUID using binary search + linear scan.
   * Ported from C++ lower_bound + PQDIF_IsEqualGUID loop.
   * @param {Buffer} tag - 16-byte GUID Buffer
   * @param {number} elementType - -1 for any type
   * @returns {Element|null}
   */
  getElementByTag(tag, elementType) {
    if (elementType === undefined) elementType = -1;

    // Binary search for lower_bound
    let lo = 0;
    let hi = this.elements.length;
    while (lo < hi) {
      const mid = (lo + hi) >>> 1;
      if (this.elements[mid].getTag() && this.elements[mid].getTag().compare(tag) < 0) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }

    // Linear scan from lower_bound position for matching tags
    for (let i = lo; i < this.elements.length; i++) {
      const el = this.elements[i];
      if (!el || !el.getTag()) break;
      if (!guidEquals(el.getTag(), tag)) break;

      if (elementType === -1 || elementType === el.getElementType()) {
        return el;
      }
    }

    return null;
  }

  /**
   * Add element maintaining sorted order by tag GUID.
   * Ported from C++ Add() using upper_bound.
   * @param {Element} element
   */
  add(element) {
    if (!element) return;

    // Find upper_bound position
    const tag = element.getTag();
    let insertIdx = this.elements.length;
    for (let i = 0; i < this.elements.length; i++) {
      const elTag = this.elements[i].getTag();
      if (!tag || !elTag || elTag.compare(tag) < 0) {
        continue;
      }
      insertIdx = i;
      break;
    }

    // If tag is not null, find upper_bound
    if (tag) {
      insertIdx = this.elements.length;
      for (let i = 0; i < this.elements.length; i++) {
        const elTag = this.elements[i].getTag();
        if (!elTag || elTag.compare(tag) < 0) {
          continue;
        }
        // Found first element >= tag, this is lower_bound
        // We need upper_bound (first > tag)
        insertIdx = i;
        // Check if equal, skip past all equal elements
        while (insertIdx < this.elements.length &&
               this.elements[insertIdx].getTag() &&
               guidEquals(this.elements[insertIdx].getTag(), tag)) {
          insertIdx++;
        }
        break;
      }
    }

    this.elements.splice(insertIdx, 0, element);

    // Set element to reference the collection's record
    element.setRecord(this.getRecord());

    // Signal that the record has changed
    if (this.record) {
      this.record.setChanged(true);
    }
  }

  /**
   * Remove element at index.
   * @param {number} index
   */
  removeAt(index) {
    if (index >= 0 && index < this.elements.length) {
      const el = this.elements[index];
      if (el) {
        el.setRecord(null);
      }
      this.elements.splice(index, 1);
      if (this.record) {
        this.record.setChanged(true);
      }
    }
  }

  /**
   * Add or replace element with same tag.
   * Ported from C++ AddOrReplace().
   * @param {Element} element
   */
  addOrReplace(element) {
    if (!element) return;

    const tagToAdd = element.getTag();
    let found = false;

    if (tagToAdd) {
      // Find lower_bound
      let lo = 0;
      let hi = this.elements.length;
      while (lo < hi) {
        const mid = (lo + hi) >>> 1;
        const elTag = this.elements[mid].getTag();
        if (elTag && elTag.compare(tagToAdd) < 0) {
          lo = mid + 1;
        } else {
          hi = mid;
        }
      }

      // Check if we found a matching element
      if (lo < this.elements.length) {
        const existing = this.elements[lo];
        if (existing && existing.getTag() && guidEquals(existing.getTag(), tagToAdd)) {
          // Replace: clear record reference on old element
          existing.setRecord(null);
          this.elements[lo] = element;
          found = true;
        }
      }
    }

    if (!found) {
      this.add(element);
      return; // add() handles record assignment and change signaling
    }

    // Set the element to reference the collection's record
    element.setRecord(this.getRecord());

    // Signal that the record has changed
    if (this.record) {
      this.record.setChanged(true);
    }
  }

  // ============================================================
  // Convenience typed setters
  // ============================================================

  /**
   * Helper: create a scalar, set tag and value, add to collection.
   */
  _setScalar(tag, typeName, value, allowReplace) {
    const factory = require('./factory');
    const psc = factory.newElement(ID_ELEMENT_TYPE_SCALAR);
    if (psc) {
      psc.setTag(tag);
      psc['set' + typeName](value);
      if (allowReplace !== false) {
        this.addOrReplace(psc);
      } else {
        this.add(psc);
      }
    }
  }

  setScalarUINT2(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueUINT2', value, allowReplace);
  }

  setScalarINT2(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueINT2', value, allowReplace);
  }

  setScalarUINT4(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueUINT4', value, allowReplace);
  }

  setScalarINT4(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueINT4', value, allowReplace);
  }

  setScalarBOOL4(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueBOOL4', value, allowReplace);
  }

  setScalarREAL4(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueREAL4', value, allowReplace);
  }

  setScalarREAL8(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueREAL8', value, allowReplace);
  }

  setScalarCOMPLEX8(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueCOMPLEX8', value, allowReplace);
  }

  setScalarCOMPLEX16(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueCOMPLEX16', value, allowReplace);
  }

  setScalarGUID(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueGUID', value, allowReplace);
  }

  setScalarTimeStamp(tag, value, allowReplace) {
    this._setScalar(tag, 'ValueTimeStamp', value, allowReplace);
  }

  // ============================================================
  // Convenience typed getters
  // ============================================================

  /**
   * Helper: find scalar by tag, get typed value.
   */
  _getScalar(tag, typeName) {
    const pel = this.getElementByTag(tag, ID_ELEMENT_TYPE_SCALAR);
    if (pel && pel.getElementType() === ID_ELEMENT_TYPE_SCALAR) {
      // Use the getValue<Type>() method which returns { status, value }
      const result = pel['getValue' + typeName]();
      if (result && result.status) return { status: true, value: result.value };
    }
    return { status: false, value: null };
  }

  getScalarUINT2(tag) { return this._getScalar(tag, 'UINT2'); }
  getScalarINT2(tag) { return this._getScalar(tag, 'INT2'); }
  getScalarUINT4(tag) { return this._getScalar(tag, 'UINT4'); }
  getScalarINT4(tag) { return this._getScalar(tag, 'INT4'); }
  getScalarBOOL4(tag) { return this._getScalar(tag, 'BOOL4'); }
  getScalarREAL4(tag) { return this._getScalar(tag, 'REAL4'); }
  getScalarREAL8(tag) { return this._getScalar(tag, 'REAL8'); }
  getScalarCOMPLEX8(tag) { return this._getScalar(tag, 'COMPLEX8'); }
  getScalarCOMPLEX16(tag) { return this._getScalar(tag, 'COMPLEX16'); }
  getScalarGUID(tag) { return this._getScalar(tag, 'GUID'); }
  getScalarTimeStamp(tag) { return this._getScalar(tag, 'TimeStamp'); }

  // ============================================================
  // Convenience typed vector setters
  // ============================================================

  /**
   * Helper: create a vector, set tag and values, add to collection.
   */
  _setVector(tag, physicalType, values, allowReplace) {
    const factory = require('./factory');
    const pvect = factory.newElement(ID_ELEMENT_TYPE_VECTOR);
    if (pvect) {
      pvect.setTag(tag);
      pvect.setPhysicalType(physicalType);
      if (typeof values === 'string') {
        pvect.setValues(values);
      } else if (Array.isArray(values)) {
        pvect.setValuesArray(physicalType, values);
      }
      if (allowReplace !== false) {
        this.addOrReplace(pvect);
      } else {
        this.add(pvect);
      }
    }
  }

  setVectorString(tag, text, allowReplace) {
    this._setVector(tag, ID_PHYS_TYPE_CHAR1, text, allowReplace);
  }

  setVectorINT1(tag, values, count, allowReplace) {
    this._setVector(tag, 20, values, allowReplace);
  }

  setVectorINT2(tag, values, count, allowReplace) {
    this._setVector(tag, 21, values, allowReplace);
  }

  setVectorINT4(tag, values, count, allowReplace) {
    this._setVector(tag, 22, values, allowReplace);
  }

  setVectorUINT4(tag, values, count, allowReplace) {
    this._setVector(tag, 32, values, allowReplace);
  }

  setVectorREAL4(tag, values, count, allowReplace) {
    this._setVector(tag, 40, values, allowReplace);
  }

  setVectorREAL8(tag, values, count, allowReplace) {
    this._setVector(tag, 41, values, allowReplace);
  }

  setVectorTimeStamp(tag, values, count, allowReplace) {
    this._setVector(tag, 50, values, allowReplace);
  }

  // ============================================================
  // Convenience typed vector getters
  // ============================================================

  /**
   * Helper: find vector by tag, get values.
   */
  _getVector(tag, max) {
    const pel = this.getElementByTag(tag, ID_ELEMENT_TYPE_VECTOR);
    if (pel && pel.getElementType() === ID_ELEMENT_TYPE_VECTOR) {
      return { status: true, vector: pel };
    }
    return { status: false, vector: null };
  }

  getVectorString(tag) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      const out = { value: '' };
      const ok = vector.getValues(out);
      return ok ? out.value : '';
    }
    return '';
  }

  GetVectorString(tag, text, max) {
    const result = this.getVectorString(tag);
    if (text && typeof text === 'object') {
      text.value = result;
    }
    return result.length > 0;
  }

  getVectorINT1(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  getVectorINT2(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  getVectorINT4(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  getVectorUINT4(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  getVectorREAL4(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  getVectorREAL8(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  getVectorTimeStamp(tag, max) {
    const { status, vector } = this._getVector(tag);
    if (status && vector) {
      return vector.getValuesArray(max || vector.getCount());
    }
    return [];
  }

  /**
   * Get vector element directly.
   * @param {Buffer} tag
   * @returns {Vector|null}
   */
  getVector(tag) {
    const pel = this.getElementByTag(tag, ID_ELEMENT_TYPE_VECTOR);
    if (pel && pel.getElementType() === ID_ELEMENT_TYPE_VECTOR) {
      return pel;
    }
    return null;
  }

  /**
   * Get scalar element directly.
   * @param {Buffer} tag
   * @returns {Scalar|null}
   */
  getScalar(tag) {
    const pel = this.getElementByTag(tag, ID_ELEMENT_TYPE_SCALAR);
    if (pel && pel.getElementType() === ID_ELEMENT_TYPE_SCALAR) {
      return pel;
    }
    return null;
  }

  /**
   * Get collection element directly.
   * @param {Buffer} tag
   * @returns {Collection|null}
   */
  getCollection(tag) {
    const pel = this.getElementByTag(tag, ID_ELEMENT_TYPE_COLLECTION);
    if (pel && pel.getElementType() === ID_ELEMENT_TYPE_COLLECTION) {
      return pel;
    }
    return null;
  }
}

module.exports = { Collection };
