#include <glib.h>
#include <nan.h>

#include "closure.h"
#include "macros.h"
#include "loop.h"
#include "type.h"
#include "value.h"

using v8::Context;
using v8::Function;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Value;
using Nan::Persistent;

namespace GNodeJS {

// locking and signalling adapted from https://github.com/node-ffi-napi/node-ffi-napi
uv_async_t Closure::asyncHandle;

GClosure *Closure::New(Local<Function> function) {
    Closure *closure = (Closure *) g_closure_new_simple (sizeof (*closure), NULL);
    closure->persistent.Reset(function);
    GClosure *gclosure = &closure->base;
    g_closure_set_marshal (gclosure, Closure::Marshal);
    g_closure_add_invalidate_notifier (gclosure, NULL, Closure::Invalidated);
    return gclosure;
}

void Closure::Execute(const Nan::Persistent<v8::Function>& persFn, GValue *g_return_value, uint n_param_values, const GValue *param_values) {
    Nan::HandleScope scope;
    auto fn = Nan::New<Function>(persFn);

    uint n_js_args = n_param_values - 1;
    #ifndef __linux__
        Local<Value>* js_args = new Local<Value>[n_js_args];
    #else
        Local<Value> js_args[n_js_args];
    #endif


    for (uint i = 0; i < n_js_args; i++) {
        bool mustCopy = true; // TODO: get information about mustCopy
        js_args[i] = GValueToV8(&param_values[i + 1], mustCopy);
    }

    Local<Object> self = fn;
    Local<Value> return_value;

    Nan::TryCatch try_catch;

    auto result = Nan::Call(fn, self, n_js_args, js_args);

    if (!try_catch.HasCaught()
            && result.ToLocal(&return_value)) {
        if (g_return_value) {
            if (G_VALUE_TYPE(g_return_value) == G_TYPE_INVALID)
                WARN ("Marshal: return value has invalid g_type");
            else if (!V8ToGValue (g_return_value, return_value, true))
                WARN ("Marshal: could not convert return value");
        }
        CallMicrotaskHandlers();
    }
    else {
        GNodeJS::QuitLoopStack();
        Nan::FatalException(try_catch);
    }

    #ifndef __linux__
        delete[] js_args;
    #endif
}

void Closure::Marshal(GClosure     *base,
                      GValue       *g_return_value,
                      uint          n_param_values,
                      const GValue *param_values,
                      gpointer      invocation_hint,
                      gpointer      marshal_data) {

    auto closure = (Closure *) base;

    AsyncCallEnvironment* env = reinterpret_cast<AsyncCallEnvironment *>(Closure::asyncHandle.data);
    uv_thread_t thread = uv_thread_self();

    /* Case 1: same thread */
    if (uv_thread_equal(&thread, &env->mainThread)) {
        Closure::Execute(closure->persistent, g_return_value, n_param_values, param_values);
        return;
    }

    /* Case 2: different thread */
    CallbackWrapper cb = CallbackWrapper();
    cb.Prepare(&closure->persistent, g_return_value, n_param_values, param_values);

    uv_mutex_lock(&env->mutex);
    env->queue.push(&cb);
    uv_mutex_unlock(&env->mutex);
    uv_async_send(&Closure::asyncHandle);

    cb.Wait();
}

void Closure::Invalidated (gpointer data, GClosure *base) {
    Closure *closure = (Closure *) base;
    closure->~Closure();
}

void Closure::QueueHandler(uv_async_t* handle) {
    AsyncCallEnvironment* data = reinterpret_cast<AsyncCallEnvironment *>(handle->data);
    uv_mutex_lock(&data->mutex);

    while (!data->queue.empty()) {
        CallbackWrapper* cb = data->queue.front();
        cb->Execute();
        data->queue.pop();
    }

    uv_mutex_unlock(&data->mutex);
}

void Closure::Initialize() {
    auto& handle = Closure::asyncHandle;
    AsyncCallEnvironment* env = new AsyncCallEnvironment();
    handle.data = env;
    env->mainThread = uv_thread_self();
    uv_loop_t* loop = uv_default_loop();
    uv_async_init(loop, &handle, Closure::QueueHandler);
    uv_mutex_init(&env->mutex);
    uv_unref(reinterpret_cast<uv_handle_t *>(&handle));
    uv_async_send(&handle);
}


CallbackWrapper::CallbackWrapper() {
    uv_mutex_init(&mutex);
    uv_mutex_lock(&mutex);
    uv_cond_init(&cond);
}

CallbackWrapper::~CallbackWrapper() {
    uv_mutex_unlock(&mutex);
    uv_cond_destroy(&cond);
    uv_mutex_destroy(&mutex);
}

void CallbackWrapper::Execute() {
    Closure::Execute(*persistent, returnValue, nValues, values);
    Done();
}
void CallbackWrapper::Prepare(const Nan::Persistent<v8::Function>* persistentIn, GValue* returnValueIn, uint nValuesIn, const GValue* valuesIn) {
    // copy values
    persistent = persistentIn;
    returnValue = returnValueIn;
    nValues = nValuesIn;
    values = new GValue[nValues];
    for (uint i = 0; i < nValues; ++i) {
        values[i] = G_VALUE_INIT;
        g_value_init(&values[i], G_VALUE_TYPE(&valuesIn[i]));
        g_value_copy(&valuesIn[i], &values[i]);
    }
}

void CallbackWrapper::Done() {
    uv_mutex_lock(&mutex);
    uv_cond_signal(&cond);
    uv_mutex_unlock(&mutex);
}
void CallbackWrapper::Wait() {
    uv_cond_wait(&cond, &mutex);
}

};
