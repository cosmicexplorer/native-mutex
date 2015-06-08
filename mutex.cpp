#include "mutex.hpp"
#include <iostream>
#include <chrono>

using namespace v8;

Persistent<Function> Mutex::constructor;

Mutex * Mutex::GetCorrectMutexType(bool recursive, bool timed) {
  if (recursive) {
    if (timed) {
      return new RecursiveTimedMutex;
    } else {
      return new RecursiveMutex;
    }
  } else {
    if (timed) {
      return new TimedMutex;
    } else {
      return new StandardMutex;
    }
  }
}

Mutex::Mutex(bool timed) : isTimed(timed) {
}

Mutex::~Mutex() {
}

StandardMutex::StandardMutex() : Mutex(false), isLocked(false) {
}
StandardMutex::~StandardMutex() {
}
RecursiveMutex::RecursiveMutex() : Mutex(false), numLocks(0) {
}
RecursiveMutex::~RecursiveMutex() {
}
TimedMutex::TimedMutex() : Mutex(true), isLocked(false) {
}
TimedMutex::~TimedMutex() {
}
RecursiveTimedMutex::RecursiveTimedMutex() : Mutex(true), numLocks(0) {
}
RecursiveTimedMutex::~RecursiveTimedMutex() {
}

void StandardMutex::lock() {
  mut.lock();
  isLocked = true;
}
void RecursiveMutex::lock() {
  mut.lock();
  ++numLocks;
}
void TimedMutex::lock() {
  mut.lock();
  isLocked = true;
}
void RecursiveTimedMutex::lock() {
  mut.lock();
  ++numLocks;
}

bool StandardMutex::tryLock() {
  bool succeeded = mut.try_lock();
  if (succeeded) {
    isLocked = true;
  }
  return succeeded;
}
bool RecursiveMutex::tryLock() {
  bool succeeded = mut.try_lock();
  if (succeeded) {
    ++numLocks;
  }
  return succeeded;
}
bool TimedMutex::tryLock() {
  bool succeeded = mut.try_lock();
  if (succeeded) {
    isLocked = true;
  }
  return succeeded;
}
bool RecursiveTimedMutex::tryLock() {
  bool succeeded = mut.try_lock();
  if (succeeded) {
    ++numLocks;
  }
  return succeeded;
}

bool StandardMutex::unlock() {
  if (isLocked) {
    mut.unlock();
    isLocked = false;
    return true;
  }
  return false;
}
bool RecursiveMutex::unlock() {
  if (numLocks > 0) {
    mut.unlock();
    --numLocks;
    return true;
  }
  return false;
}
bool TimedMutex::unlock() {
  if (isLocked) {
    mut.unlock();
    isLocked = false;
    return true;
  }
  return false;
}
bool RecursiveTimedMutex::unlock() {
  if (numLocks > 0) {
    mut.unlock();
    --numLocks;
    return true;
  }
  return false;
}

bool TimedMutex::tryLockFor(size_t ms) {
  bool succeeded = mut.try_lock_for(std::chrono::milliseconds(ms));
  if (succeeded) {
    isLocked = true;
  }
  return succeeded;
}
bool RecursiveTimedMutex::tryLockFor(size_t ms) {
  bool succeeded = mut.try_lock_for(std::chrono::milliseconds(ms));
  if (succeeded) {
    ++numLocks;
  }
  return succeeded;
}

void Mutex::Init(Handle<Object> exports) {
  Isolate * isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Mutex"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "tryLock", TryLock);
  NODE_SET_PROTOTYPE_METHOD(tpl, "lock", Lock);
  NODE_SET_PROTOTYPE_METHOD(tpl, "unlock", Unlock);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tryLockFor", TryLockFor);
  NODE_SET_PROTOTYPE_METHOD(tpl, "doWithLock", DoWithLock);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Mutex"), tpl->GetFunction());
}

void Mutex::New(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Mutex(...)`
    const int len = args.Length();
    bool recursive, timed;
    if (len == 0 or not args[0]->IsObject()) {
      recursive = false;
      timed = false;
    } else {
      Local<Object> opts = args[0]->ToObject();
      Local<Value> recursiveOpt =
       opts->Get(String::NewFromUtf8(isolate, "recursive"));
      recursive = recursiveOpt->IsBoolean() and recursiveOpt->BooleanValue();
      Local<Value> timedOpt = opts->Get(String::NewFromUtf8(isolate, "timed"));
      timed = timedOpt->IsBoolean() and timedOpt->BooleanValue();
    }
    Mutex * obj = Mutex::GetCorrectMutexType(recursive, timed);
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

void Mutex::TryLock(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Mutex * obj = ObjectWrap::Unwrap<Mutex>(args.Holder());
  bool succeeded(obj->tryLock());
  args.GetReturnValue().Set(Boolean::New(isolate, succeeded));
}

void Mutex::Lock(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Mutex * obj = ObjectWrap::Unwrap<Mutex>(args.Holder());
  obj->lock();
  args.GetReturnValue().Set(Null(isolate));
}

void Mutex::Unlock(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Mutex * obj = ObjectWrap::Unwrap<Mutex>(args.Holder());
  bool already_locked(obj->unlock());
  if (not already_locked) {
    isolate->ThrowException(Exception::TypeError(
     String::NewFromUtf8(isolate, "You don't have the lock!")));
  }
  args.GetReturnValue().Set(Null(isolate));
}

static ssize_t getNumArgs(const FunctionCallbackInfo<Value> & args) {
  if (args.Length() == 0 or not args[0]->IsUint32()) {
    return -1;
  } else {
    return args[0]->Uint32Value();
  }
}

void Mutex::TryLockFor(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Mutex * obj = ObjectWrap::Unwrap<Mutex>(args.Holder());
  obj->tryLockFor(args, isolate);
}

void Mutex::tryLockFor(const FunctionCallbackInfo<Value> & args,
                       v8::Isolate * isolate) {
  isolate->ThrowException(Exception::TypeError(
   String::NewFromUtf8(isolate, "This isn't a timed mutex!")));
}

void TimedMutex::tryLockFor(const FunctionCallbackInfo<Value> & args,
                            v8::Isolate * isolate) {
  const ssize_t num_args(getNumArgs(args));
  if (-1 == num_args) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(
     isolate,
     "You must specify a millisecond timeout when using tryLockFor!")));
  } else {
    args.GetReturnValue().Set(Boolean::New(isolate, tryLockFor(num_args)));
  }
}

void RecursiveTimedMutex::tryLockFor(const FunctionCallbackInfo<Value> & args,
                                     v8::Isolate * isolate) {
  const ssize_t num_args(getNumArgs(args));
  if (-1 == num_args) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(
     isolate,
     "You must specify a millisecond timeout when using tryLockFor!")));
  } else {
    args.GetReturnValue().Set(Boolean::New(isolate, tryLockFor(num_args)));
  }
}

void Mutex::DoWithLock(const FunctionCallbackInfo<Value> & args) {
  Isolate * isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  if (args.Length() < 2 or not args[0]->IsBoolean() or
      not args[1]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
     String::NewFromUtf8(isolate, "Invalid arguments given to doWithLock!")));
  } else {
    Mutex * obj = ObjectWrap::Unwrap<Mutex>(args.Holder());
    if (args[0]->BooleanValue()) {
      obj->lock();
    }
    Local<Function> cb = Local<Function>::Cast(args[1]);
    const size_t argc(0);
    Local<Value> argv[argc] = {};
    Local<Value> res =
     cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    bool already_locked(obj->unlock());
    if (not already_locked) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(
       isolate,
       "You didn't acquire the lock before calling doWithLock(false, cb)!")));
    }
    args.GetReturnValue().Set(res);
  }
}
