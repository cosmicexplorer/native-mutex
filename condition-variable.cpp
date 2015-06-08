#include "condition-variable.hpp"
#include <iostream>

using namespace v8;

Persistent<Function> ConditionVariable::constructor;

ConditionVariable::ConditionVariable() : mut(), cv() {
}

ConditionVariable::~ConditionVariable() {
}

void ConditionVariable::Init(Handle<Object> exports) {
  Isolate * isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "ConditionVariable"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "wait", Wait);
  NODE_SET_PROTOTYPE_METHOD(tpl, "waitFor", WaitFor);
  NODE_SET_PROTOTYPE_METHOD(tpl, "notifyOne", NotifyOne);
  NODE_SET_PROTOTYPE_METHOD(tpl, "notifyAll", NotifyAll);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "ConditionVariable"),
               tpl->GetFunction());
}

void ConditionVariable::New(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new ConditionVariable(...)`
    ConditionVariable * obj = new ConditionVariable();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `ConditionVariable(...)`, turn into construct
    // call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void ConditionVariable::Wait(const v8::FunctionCallbackInfo<v8::Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  ConditionVariable * obj = new ConditionVariable();
  if (args.Length() == 1 and not args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
     String::NewFromUtf8(isolate, "No predicate given for wait!")));
  } else {
    std::unique_lock<std::mutex> u_lock(obj->mut);
    if (args.Length() == 1) {
      Local<Function> cb = Local<Function>::Cast(args[0]);
      const size_t argc = 0;
      Local<Value> argv[argc] = {};
      /* blocks when predicate is false */
      obj->cv.wait(u_lock, [&]() {
        Local<Value> res =
         cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
        /* if predicate doesn't return a value, or returns non-bool, it's true
         */
        return not res->IsBoolean() or res->BooleanValue();
      });
    } else {
      obj->cv.wait(u_lock);
    }
    args.GetReturnValue().Set(Null(isolate));
  }
}
void ConditionVariable::WaitFor(
 const v8::FunctionCallbackInfo<v8::Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  ConditionVariable * obj = new ConditionVariable();
  if (args.Length() == 0) {
    isolate->ThrowException(Exception::TypeError(
     String::NewFromUtf8(isolate, "no arguments given to waitFor!")));
  } else if (not args[0]->IsUint32()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(
     isolate, "invalid milliseconds argument to waitFor (not an integer)!")));
  } else if (args.Length() >= 2 and not args[1]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(
     isolate, "invalid callback argument to waitFor (not a function)!")));
  } else {
    std::unique_lock<std::mutex> u_lock(obj->mut);
    size_t timeout = args[0]->Uint32Value();
    std::cv_status result;
    bool res_boolean;
    if (args.Length() == 1) {
      result = obj->cv.wait_for(u_lock, std::chrono::milliseconds(timeout));
    } else {
      const size_t argc = 0;
      Local<Value> argv[argc] = {};
      Local<Function> cb = Local<Function>::Cast(args[1]);
      res_boolean =
       obj->cv.wait_for(u_lock, std::chrono::milliseconds(timeout), [&]() {
         Local<Value> res =
          cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
         return not res->IsBoolean() or res->BooleanValue();
       });
    }
    bool did_timeout;
    if (args.Length() == 1) {
      did_timeout = std::cv_status::timeout == result;
    } else {
      did_timeout = res_boolean;
    }
    args.GetReturnValue().Set(Boolean::New(isolate, did_timeout));
  }
}
void ConditionVariable::NotifyOne(
 const v8::FunctionCallbackInfo<v8::Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  ConditionVariable * obj = new ConditionVariable();
  std::unique_lock<std::mutex> u_lock(obj->mut);
  obj->cv.notify_one();
  args.GetReturnValue().Set(Null(isolate));
}
void ConditionVariable::NotifyAll(
 const v8::FunctionCallbackInfo<v8::Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  ConditionVariable * obj = new ConditionVariable();
  std::unique_lock<std::mutex> u_lock(obj->mut);
  obj->cv.notify_all();
  args.GetReturnValue().Set(Null(isolate));
}
