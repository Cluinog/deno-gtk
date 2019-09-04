/* autogenerated by generator-font-options.js */

#include "../../debug.h"
#include "../../gi.h"
#include "../../util.h"
#include "font-options.h"

using namespace v8;


namespace GNodeJS {

namespace Cairo {



Nan::Persistent<FunctionTemplate> FontOptions::constructorTemplate;
Nan::Persistent<Function>         FontOptions::constructor;



/*
 * Initialize
 */

FontOptions::FontOptions(cairo_font_options_t* data) : ObjectWrap() {
  _data = data;
}

/*
 * Destroy
 */

FontOptions::~FontOptions() {
  if (_data != NULL) {
    cairo_font_options_destroy (_data);
  }
}


/*
 * Template methods
 */


Local<FunctionTemplate> FontOptions::GetTemplate() {
  if (constructorTemplate.IsEmpty())
    FontOptions::SetupTemplate();
  return Nan::New<FunctionTemplate> (constructorTemplate);
}

Local<Function> FontOptions::GetConstructor() {
  if (constructor.IsEmpty())
    FontOptions::SetupTemplate();
  return Nan::New<Function> (constructor);
}

void FontOptions::SetupTemplate() {

  // Constructor
  auto tpl = Nan::New<FunctionTemplate>(FontOptions::New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(Nan::New("CairoFontOptions").ToLocalChecked());


  SET_PROTOTYPE_METHOD(tpl, status);
  SET_PROTOTYPE_METHOD(tpl, merge);
  SET_PROTOTYPE_METHOD(tpl, hash);
  SET_PROTOTYPE_METHOD(tpl, equal);
  SET_PROTOTYPE_METHOD(tpl, setAntialias);
  SET_PROTOTYPE_METHOD(tpl, getAntialias);
  SET_PROTOTYPE_METHOD(tpl, setSubpixelOrder);
  SET_PROTOTYPE_METHOD(tpl, getSubpixelOrder);
  SET_PROTOTYPE_METHOD(tpl, setHintStyle);
  SET_PROTOTYPE_METHOD(tpl, getHintStyle);
  SET_PROTOTYPE_METHOD(tpl, setHintMetrics);
  SET_PROTOTYPE_METHOD(tpl, getHintMetrics);
  SET_PROTOTYPE_METHOD(tpl, getVariations);
  SET_PROTOTYPE_METHOD(tpl, setVariations);

  auto ctor = Nan::GetFunction (tpl).ToLocalChecked();



  constructorTemplate.Reset(tpl);
  constructor.Reset(ctor);



}



/*
 * Initialize method
 */


void FontOptions::Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
  Nan::Set (target, Nan::New ("FontOptions").ToLocalChecked(), FontOptions::GetConstructor());
}



/*
 * Instance constructors
 */



NAN_METHOD(FontOptions::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError("Class constructors cannot be invoked without 'new'");
  }

  cairo_font_options_t* data = NULL;

  if (info[0]->IsExternal()) {
    data = (cairo_font_options_t*) External::Cast (*info[0])->Value ();
  }
  else if (info.Length() == 1) {
    auto original = Nan::ObjectWrap::Unwrap<FontOptions>(info[0].As<Object>())->_data;

    data = cairo_font_options_copy (original);
  }
  else {
    data = cairo_font_options_create ();
  }

  FontOptions* instance = new FontOptions(data);
  instance->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}



/*
 * Methods
 */

NAN_METHOD(FontOptions::status) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  cairo_status_t result = cairo_font_options_status (options);

  // return
  Local<Value> returnValue = Nan::New (result);
  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(FontOptions::merge) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto other = Nan::ObjectWrap::Unwrap<FontOptions>(info[0].As<Object>())->_data;

  // function call
  cairo_font_options_merge (options, other);
}

NAN_METHOD(FontOptions::hash) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  unsigned long result = cairo_font_options_hash (options);

  // return
  Local<Value> returnValue = Nan::New ((double) result);
  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(FontOptions::equal) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto other = Nan::ObjectWrap::Unwrap<FontOptions>(info[0].As<Object>())->_data;

  // function call
  cairo_bool_t result = cairo_font_options_equal (options, other);

  // return
  Local<Value> returnValue = Nan::New ((bool) result);
  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(FontOptions::setAntialias) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto antialias = (cairo_antialias_t) Nan::To<int64_t>(info[0].As<Number>()).ToChecked();

  // function call
  cairo_font_options_set_antialias (options, antialias);
}

NAN_METHOD(FontOptions::getAntialias) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  cairo_antialias_t result = cairo_font_options_get_antialias (options);

  // return
  Local<Value> returnValue = Nan::New (result);
  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(FontOptions::setSubpixelOrder) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto subpixel_order = (cairo_subpixel_order_t) Nan::To<int64_t>(info[0].As<Number>()).ToChecked();

  // function call
  cairo_font_options_set_subpixel_order (options, subpixel_order);
}

NAN_METHOD(FontOptions::getSubpixelOrder) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  cairo_subpixel_order_t result = cairo_font_options_get_subpixel_order (options);

  // return
  Local<Value> returnValue = Nan::New (result);
  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(FontOptions::setHintStyle) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto hint_style = (cairo_hint_style_t) Nan::To<int64_t>(info[0].As<Number>()).ToChecked();

  // function call
  cairo_font_options_set_hint_style (options, hint_style);
}

NAN_METHOD(FontOptions::getHintStyle) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  cairo_hint_style_t result = cairo_font_options_get_hint_style (options);

  // return
  Local<Value> returnValue = Nan::New (result);
  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(FontOptions::setHintMetrics) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto hint_metrics = (cairo_hint_metrics_t) Nan::To<int64_t>(info[0].As<Number>()).ToChecked();

  // function call
  cairo_font_options_set_hint_metrics (options, hint_metrics);
}

NAN_METHOD(FontOptions::getHintMetrics) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  cairo_hint_metrics_t result = cairo_font_options_get_hint_metrics (options);

  // return
  Local<Value> returnValue = Nan::New (result);
  info.GetReturnValue().Set(returnValue);
}

#if CAIRO_VERSION_MAJOR >= 1 && CAIRO_VERSION_MINOR >= 16
NAN_METHOD(FontOptions::getVariations) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // function call
  const char * result = cairo_font_options_get_variations (options);

  // return
  Local<Value> returnValue = UTF8 (result);
  info.GetReturnValue().Set(returnValue);
}
#endif

#if CAIRO_VERSION_MAJOR >= 1 && CAIRO_VERSION_MINOR >= 16
NAN_METHOD(FontOptions::setVariations) {
  auto self = info.This();
  auto options = Nan::ObjectWrap::Unwrap<FontOptions>(self)->_data;

  // in-arguments
  auto variations = *Nan::Utf8String (info[0].As<String>());

  // function call
  cairo_font_options_set_variations (options, variations);
}
#endif



}; // Cairo

}; // GNodeJS