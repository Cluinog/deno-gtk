/*
 * native.js
 */
import binary from 'npm:@mapbox/node-pre-gyp';
import path from 'node:path';
const __dirname = new URL('.', import.meta.url).pathname;
const packagePath = path.resolve(path.join(__dirname,'../package.json'))
const bindingPath = binary.find(packagePath)

const binding = require(bindingPath)

module.exports = binding
