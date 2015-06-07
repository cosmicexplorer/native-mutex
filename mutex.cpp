#include "mutex.hpp"

using namespace v8;

/* Mutex */

Persistent<Function> Mutex::constructor;

Mutex::Mutex(double value) : value_(value) {}

Mutex::~Mutex() {}

void Mutex::Init(Handle<Object> exports) {
  Isolate *isolate = Isolate::GetCurrent();

  /* isolate->ThrowException(
     Exception::TypeError(String::NewFromUtf8(isolate, "wrong type!"))); */

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Mutex"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Mutex"), tpl->GetFunction());
}

void Mutex::New(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Mutex(...)`
    /* double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue(); */
    double value(2);
    auto arg = args[0];
    if (arg->IsUndefined()) {
      value = -1;
    } else if (arg->IsObject()) {
      auto obj = Handle<Object>::Cast(arg);
      auto result = obj->Get(String::NewFromUtf8(isolate, "type"));
      if (result->IsNumber()) {
        value = result->NumberValue();
      }
    }
    Mutex *obj = new Mutex(value);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `Mutex(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Mutex::PlusOne(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Mutex *obj = ObjectWrap::Unwrap<Mutex>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

/* RecursiveMutex */

Persistent<Function> RecursiveMutex::constructor;

RecursiveMutex::RecursiveMutex(double value) : value_(value) {}

RecursiveMutex::~RecursiveMutex() {}

void RecursiveMutex::Init(Handle<Object> exports) {
  Isolate *isolate = Isolate::GetCurrent();

  /* isolate->ThrowException(
     Exception::TypeError(String::NewFromUtf8(isolate, "wrong type!"))); */

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "RecursiveMutex"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "RecursiveMutex"),
               tpl->GetFunction());
}

void RecursiveMutex::New(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new RecursiveMutex(...)`
    /* double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue(); */
    double value(2);
    auto arg = args[0];
    if (arg->IsUndefined()) {
      value = -1;
    } else if (arg->IsObject()) {
      auto obj = Handle<Object>::Cast(arg);
      auto result = obj->Get(String::NewFromUtf8(isolate, "type"));
      if (result->IsNumber()) {
        value = result->NumberValue();
      }
    }
    RecursiveMutex *obj = new RecursiveMutex(value);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `RecursiveMutex(...)`, turn into construct
    // call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void RecursiveMutex::PlusOne(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  RecursiveMutex *obj = ObjectWrap::Unwrap<RecursiveMutex>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

/* TimeoutMutex */

Persistent<Function> TimeoutMutex::constructor;

TimeoutMutex::TimeoutMutex(double value) : value_(value) {}

TimeoutMutex::~TimeoutMutex() {}

void TimeoutMutex::Init(Handle<Object> exports) {
  Isolate *isolate = Isolate::GetCurrent();

  /* isolate->ThrowException(
     Exception::TypeError(String::NewFromUtf8(isolate, "wrong type!"))); */

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "TimeoutMutex"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "TimeoutMutex"),
               tpl->GetFunction());
}

void TimeoutMutex::New(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new TimeoutMutex(...)`
    /* double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue(); */
    double value(2);
    auto arg = args[0];
    if (arg->IsUndefined()) {
      value = -1;
    } else if (arg->IsObject()) {
      auto obj = Handle<Object>::Cast(arg);
      auto result = obj->Get(String::NewFromUtf8(isolate, "type"));
      if (result->IsNumber()) {
        value = result->NumberValue();
      }
    }
    TimeoutMutex *obj = new TimeoutMutex(value);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `TimeoutMutex(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void TimeoutMutex::PlusOne(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  TimeoutMutex *obj = ObjectWrap::Unwrap<TimeoutMutex>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}
