/*
 * loop.js
 */

import internal from './native.js';
import process from "node:process";

export default start;
export {
  start,
}


let didStart = false
const nextTick = process.nextTick
const setTimeout = globalThis.setTimeout
const setInterval = globalThis.setInterval
const setImmediate = globalThis.setImmediate

/**
 * Starts the loops integration
 */
function start() {
  if (didStart)
    return
  didStart = true

  process.nextTick = wrappedLoopFunction(nextTick)
  globalThis.setTimeout = wrappedLoopFunction(setTimeout)
  globalThis.setInterval = wrappedLoopFunction(setInterval)
  globalThis.setImmediate = wrappedLoopFunction(setImmediate)

  internal.StartLoop()
}


// Helpers

function wrappedLoopFunction(fn) {
  return (callback, ...rest) => {
    return fn(tryCallback(callback), ...rest)
  }
}

function tryCallback(fn) {
  return (...args) => {
    try {
      return fn(...args)
    } catch (err) {
      console.error(err)
      process.exit(1)
    }
  }
}
