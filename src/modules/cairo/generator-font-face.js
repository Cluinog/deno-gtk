/*
 * generator-font-face.js
 */

const fs = require('fs')
const path = require('path')
const util = require('util')
const removeTrailingSpaces = require('remove-trailing-spaces')
const { unindent } = require('./indent.js')

const {
  generateClassMethodSource,
  generateSource,
  parseFile,
  getJSName,
} = require('./generator.js')


util.inspect.defaultOptions = { depth: 6 }

const base = {
  name: 'FontFace',
  constructor: null,
  functions: null,
  isBase: true,
  type: 'cairo_font_face_t',
  prefix: /cairo_(([a-z0-9]+_)?font_face)?/,
  destroy: 'cairo_font_face_destroy',
  filename: __filename,
  dependencies: [
    'path.h',
    'font-face.h',
    'matrix.h',
    'surface.h',
  ],
}

generateCairoFontFace()

function generateCairoFontFace() {
  const result = parseFile(path.join(__dirname, 'font-face.nid'))
  const declarations = result.declarations

  console.log(declarations)

  const namespaces = declarations.map((cur) => {
    const name = cur.namespace.name
    const options = name === base.name ? base : {
      name,
      constructor: null,
      functions: null,
      isBase: false,
      type: base.type,
      prefix: base.prefix,
      base,
    }

    const allFunctions =
      cur.namespace.declarations
        .filter(d => d.function)
        .map(d => {
          const fn = d.function
          fn.source = generateClassMethodSource(fn, options)
          return fn
        })

    options.constructor = allFunctions.find(fn => fn.attributes.constructor === true)
    options.functions = allFunctions.filter(fn => fn.attributes.constructor !== true)

    return options
  })


  /* ToyFontFace
   * FtFontFace
   * Win32FontFace
   * QuartzFontFace */

  const header = generateHeader(namespaces)
  const source = generateSource(base, namespaces)

  fs.writeFileSync(path.join(__dirname, 'font-face.h'),  header)
  fs.writeFileSync(path.join(__dirname, 'font-face.cc'), source)
}

// Helpers

function generateHeader(namespaces) {
  const classDeclarations = namespaces.map(generateClassDeclaration)
  return removeTrailingSpaces(unindent(`

    /* autogenerated by ${path.basename(__filename)} */

    #pragma once

    #include <nan.h>
    #include <node.h>
    #include <girepository.h>
    #include <glib.h>
    #include <cairo.h>
    #include <cairo-ft.h>
    #ifdef PLATFORM_WIN
    #   include <cairo-win32.h>
    #endif
    #ifdef PLATFORM_MAC
    #   include <cairo-quartz.h>
    #endif

    namespace GNodeJS {

    namespace Cairo {

    ${classDeclarations.join('\n    ')}

    }; // Cairo

    }; // GNodeJS

  `))
}

function generateClassDeclaration(options) {

  return `
    class ${options.name}: public ${options.isBase ? 'Nan::ObjectWrap' : 'FontFace'} {
      public:
        static Nan::Persistent<v8::FunctionTemplate> constructorTemplate;
        static Nan::Persistent<v8::Function>         constructor;
        static void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);${options.isBase ? `
        static void SetupTemplate();` : `
        static void SetupTemplate(Local<v8::FunctionTemplate> parentTpl);`}
        static Local<v8::FunctionTemplate> GetTemplate();
        static Local<v8::Function> GetConstructor();

        static NAN_METHOD(New);

        ${options.functions.map(fn =>
          (fn.attributes.ifdef ? `#ifdef ${fn.attributes.ifdef}\n        ` : '')
          + `static NAN_METHOD(${getFunctionJSName(fn)});`
          + (fn.attributes.ifdef ? `\n        #endif` : '')
          ).join('\n        ')}

        ${options.isBase ? `
        ${options.name}(cairo_font_face_t* data);
        ~${options.name}();

        cairo_font_face_t* _data;
` : `
        ${options.name}(cairo_font_face_t* data) : FontFace(data) {};
`}    };`
}

function getFunctionJSName(fn) {
  return getJSName(fn.name, /cairo_(([a-z0-9]+_)?font_face)?/)
}
