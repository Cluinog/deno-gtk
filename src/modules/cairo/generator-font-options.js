/*
 * generetor-font-options.js
 */

const fs = require('fs')
const path = require('path')
const util = require('util')
const removeTrailingSpaces = require('remove-trailing-spaces')
const { indent, unindent } = require('./indent.js')

util.inspect.defaultOptions = { depth: 6 }

const {
  generateClassMethodSource,
  generateSource,
  getInArgumentSource,
  parseFile,
  getJSName,
  addVersionGuard,
} = require('./generator.js')


const options = {
  name: 'FontOptions',
  isBase: true,
  constructor: undefined,
  functions: undefined,
  type: 'cairo_font_options_t',
  prefix: /cairo_font_options/,
  create: 'cairo_font_options_create',
  destroy: 'cairo_font_options_destroy',
  filename: __filename,
  dependencies: [
    'font-options.h',
  ],
}



generateCairoFontOptions()

function generateCairoFontOptions() {
  const result = parseFile(path.join(__dirname, 'font-options.nid'))
  const declarations = result.declarations

  options.constructor = declarations.find(d => d.function && d.function.attributes.constructor).function
  options.functions =
    declarations
      .filter(d => d.function && d.function.attributes.constructor !== true)
      .map(d => {
        const fn = d.function
        fn.source = generateClassMethodSource(fn, options)
        return fn
      })

  const header = generateHeader(options)
  const source = generateSource(options, [options])

  fs.writeFileSync(path.join(__dirname, 'font-options.h'),  header)
  fs.writeFileSync(path.join(__dirname, 'font-options.cc'), source)
}

// Helpers

function generateHeader(options) {
  const classDeclaration = generateClassDeclaration(options)

  return removeTrailingSpaces(unindent(`

    /* autogenerated by ${path.basename(__filename)} */

    #pragma once

    #include <nan.h>
    #include <node.h>
    #include <girepository.h>
    #include <glib.h>
    #include <cairo.h>

    namespace GNodeJS {

    namespace Cairo {

    ${classDeclaration}

    }; // Cairo

    }; // GNodeJS

  `))
}

function generateClassDeclaration(options) {

  return `
    class ${options.name}: public Nan::ObjectWrap {
      public:
        static Nan::Persistent<v8::FunctionTemplate> constructorTemplate;
        static Nan::Persistent<v8::Function>         constructor;
        static void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);
        static void SetupTemplate();
        static Local<v8::FunctionTemplate> GetTemplate();
        static Local<v8::Function> GetConstructor();

        static NAN_METHOD(New);

        ${options.functions.map(fn =>
          addVersionGuard(fn, `static NAN_METHOD(${getJSName(fn.name, options.prefix)});`, '        ')
        ).join('\n        ')}

        ${options.name}(${options.type}* data);
        ~${options.name}();

        ${options.type}* _data;
    };
  `
}
