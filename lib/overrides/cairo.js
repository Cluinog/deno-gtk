/*
 * cairo.js
 */


import internal from '../native.js';

exports.apply = (cairo) => {
  internal.Cairo.init(cairo)

  cairo.FtSynthesize = {
    BOLD:    1 << 0,
    OBLIQUE: 1 << 1,
  }
}
