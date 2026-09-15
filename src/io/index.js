'use strict';

const { StreamProcessor, NothingProcessor, ZlibProcessor } = require('./streamProcessor');
const { StreamIO } = require('./streamIO');
const { FileStreamIO } = require('./fileStreamIO');
const { ChunkStreamIO } = require('./chunkStreamIO');

module.exports = {
  StreamProcessor,
  NothingProcessor,
  ZlibProcessor,
  StreamIO,
  FileStreamIO,
  ChunkStreamIO,
};
