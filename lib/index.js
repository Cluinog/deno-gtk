/*
 * index.js
 */

//const internal = require('./native.js')
import internal from './native.js';
const moduleCache = internal.GetModuleCache()

// Must be loaded first, to setup the GI functions
import bootstrap from './bootstrap.js';
import module_ from './module.js';
import loop from './loop.js';

import registerClass from './register-class.js';

/*
 * Exports
 */

module.exports = {
  // Public API
  ...module_,
  startLoop: loop.start,
  registerClass: registerClass,
  System: internal.System,

  // Private API
  _cache: moduleCache,
  _GIRepository: bootstrap.GI,
  _InfoType: bootstrap.GI.InfoType,
}

