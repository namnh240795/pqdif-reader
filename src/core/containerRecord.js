'use strict';

const { Record } = require('./record');
const { PqdifInfo, guidEquals } = require('../info/info');
const { ID_PHYS_TYPE_UNS_INTEGER4 } = require('../constants/physicalTypes');
const {
  tagCompressionStyleID, tagCompressionAlgorithmID,
  tagLanguage, tagTitle, tagSubject, tagAuthor, tagKeywords, tagComments,
  tagLastSavedBy, tagApplication, tagSecurity, tagOwner, tagCopyright,
  tagTrademarks, tagNotes,
} = require('../constants/tagGuids');

/**
 * CPQDIF_R_Container - container record (always first record).
 * Ported from rec_container.h/rec_container.cpp
 */
class ContainerRecord extends Record {
  constructor() {
    super();
  }

  /**
   * Get compression info.
   * @param {Object} out - { styleComp, algComp }
   * @returns {boolean}
   */
  getCompressionInfo(out) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    // Check record type
    const tagRecord = this.getHeaderTag();
    if (!tagRecord || !guidEquals(tagRecord, require('../constants/tagGuids').tagContainer)) {
      return false;
    }

    const outStyle = { value: 0 };
    const status = Record.getScalarValueInCollection(pcollMain, tagCompressionStyleID, ID_PHYS_TYPE_UNS_INTEGER4, outStyle);
    if (status) {
      out.styleComp = outStyle.value;
      const outAlg = { value: 0 };
      const status2 = Record.getScalarValueInCollection(pcollMain, tagCompressionAlgorithmID, ID_PHYS_TYPE_UNS_INTEGER4, outAlg);
      if (status2) {
        out.algComp = outAlg.value;
        return true;
      }
    }
    return false;
  }

  /**
   * GetCompressionInfo C++ style.
   * @param {Object} out - { styleComp, algComp }
   * @returns {boolean}
   */
  GetCompressionInfo(out) {
    return this.getCompressionInfo(out);
  }

  /**
   * Set container metadata strings.
   * @param {string} language
   * @param {string} title
   * @param {string} subject
   * @param {string} author
   * @param {string} keywords
   * @param {string} comments
   * @param {string} lastSavedBy
   * @param {string} application
   * @param {string} security
   * @param {string} owner
   * @param {string} copyright
   * @param {string} trademarks
   * @param {string} notes
   * @returns {boolean}
   */
  setInfo(language, title, subject, author, keywords, comments,
          lastSavedBy, application, security, owner, copyright, trademarks, notes) {
    const pcollMain = this.getMainCollection();
    if (!pcollMain) return false;

    pcollMain.setVectorString(tagLanguage, language);
    pcollMain.setVectorString(tagTitle, title);
    pcollMain.setVectorString(tagSubject, subject);
    pcollMain.setVectorString(tagAuthor, author);
    pcollMain.setVectorString(tagKeywords, keywords);
    pcollMain.setVectorString(tagComments, comments);
    pcollMain.setVectorString(tagLastSavedBy, lastSavedBy);
    pcollMain.setVectorString(tagApplication, application);
    pcollMain.setVectorString(tagSecurity, security);
    pcollMain.setVectorString(tagOwner, owner);
    pcollMain.setVectorString(tagCopyright, copyright);
    pcollMain.setVectorString(tagTrademarks, trademarks);
    pcollMain.setVectorString(tagNotes, notes);

    return true;
  }
}

module.exports = { ContainerRecord };
