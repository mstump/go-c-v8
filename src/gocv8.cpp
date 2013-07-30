/*

  The MIT License (MIT)

  Copyright (c) 2013 Matthew Stump

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/

#include <iostream>
#include <string>
#include <v8.h>

#include "gocv8/gocv8.h"

struct gocv8_context_t
{
    gocv8_context_t() :
        v8_context(v8::Context::New())
    {}

    v8::HandleScope v8_handle_scope;
    v8::Handle<v8::Context> v8_context;
};

struct event_data_t
{
    std::string data;

    static v8::Handle<v8::Value>
    get_event_data(
        v8::Local<v8::String>   property,
        const v8::AccessorInfo& info)
    {
        (void) property;

        v8::Local<v8::Object> self = info.Holder();
        v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        std::string value = static_cast<event_data_t*>(ptr)->data;
        return v8::String::New(value.c_str(), value.size());
    }

    static void
    set_event_data(
        v8::Local<v8::String>   property,
        v8::Local<v8::Value>    value,
        const v8::AccessorInfo& info)
    {
        (void) property;

        v8::Local<v8::Object> self = info.Holder();
        v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();

        v8::String::Utf8Value string_value(value->ToString());
        static_cast<event_data_t*>(ptr)->data = std::string(*string_value);
    }
};

void*
gocv8_context_new()
{
    return new gocv8_context_t();
}

void
gocv8_context_free(
    void* context)
{
    delete static_cast<gocv8_context_t*>(context);
}

bool
gocv8_init(
    void*  context,
    void** status)
{
    (void) context;
    (void) status;
    return false;
}


inline v8::Handle<v8::Object>
wrap_event(
    event_data_t *event)
{
    v8::HandleScope scope;
    v8::Handle<v8::FunctionTemplate> class_template = v8::FunctionTemplate::New();
    v8::Handle<v8::ObjectTemplate> instance_template = class_template->InstanceTemplate();
    instance_template->SetInternalFieldCount(1);

    instance_template->SetAccessor(
        v8::String::New("data"),
        &event_data_t::get_event_data,
        &event_data_t::set_event_data);

    v8::Local<v8::Object> instance = instance_template->NewInstance();
    instance->SetInternalField(0, v8::External::New(event));

    //Return the JS object representing this class
    return scope.Close(instance);
}

v8::Handle<v8::Value>
log_callback(
    const v8::Arguments& args)
{
    if (args.Length() < 1) {
        return v8::Undefined();
    }

    v8::HandleScope scope(args.GetIsolate());
    v8::Handle<v8::Value> arg = args[0];
    v8::String::Utf8Value value(arg);
    printf("v8-log: %s\n", *value);
    return v8::Undefined();
}

bool
gocv8_process_event(
    void*       context,
    const char* data,
    size_t      data_size,
    const char* js,
    size_t      js_size,
    void**      status)
{
    (void) context;
    (void) status;

    event_data_t event;
    event.data.assign(data, data_size);

    // Create a handle scope to hold the temporary references.
    // this needs to be deleted
    v8::HandleScope handle_scope(v8::Isolate::GetCurrent());

    // Create a template for the global object where we set the
    // TODO this is a memory leak
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
    global->Set(v8::String::New("log"), v8::FunctionTemplate::New(log_callback));

    // Each processor gets its own context so different processors don't
    // affect each other. Context::New returns a persistent handle which
    // is what we need for the reference to remain after we return from
    // this method. That persistent handle has to be disposed in the
    // destructor.
    // TODO this is a memory leak
    v8::Persistent<v8::Context> local_context = v8::Context::New(NULL, global);

    // Enter the new context so all the following operations take place
    // within it.
    v8::Context::Scope context_scope(local_context);

    v8::Handle<v8::Object> js_event = wrap_event(&event);
    local_context->Global()->Set(v8::String::New("event_data"), js_event, v8::ReadOnly);

    // setup error handling
    v8::TryCatch try_catch;

    // compile
    v8::Handle<v8::String> source = v8::String::New(js, js_size);
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    if (script.IsEmpty()) {
        v8::String::Utf8Value error(try_catch.Exception());
        std::cerr << "v8-error: " << *error << std::endl;
        return false;
    }

    // run
    v8::Handle<v8::Value> result = script->Run();
    if (result.IsEmpty()) {
        v8::String::Utf8Value error(try_catch.Exception());
        std::cerr << "v8-error: " << *error << std::endl;
        return false;
    }
    return true;
}
