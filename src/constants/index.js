'use strict';

const physicalTypes = require('./physicalTypes');
const elementTypes = require('./elementTypes');
const compressionIds = require('./compressionIds');
const tagGuids = require('./tagGuids');
const idGuids = require('./idGuids');
const idIntegers = require('./idIntegers');

module.exports = {
  ...physicalTypes,
  ...elementTypes,
  ...compressionIds,
  ...tagGuids,
  ...idGuids,
  ...idIntegers,
};
