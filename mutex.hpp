#ifndef MUTEX_HPP
#define MUTEX_HPP

/* these three classes wrap the similarly-named mutex classes introduced in
   c++11. why don't they inherit from each other? because v8 makes all
   javascript-facing functions static (therefore non-inheritable), so there's
   no real point except for aesthetics, and i don't do ui work. */

/* std includes */
#include <mutex>

/* node includes */
#include <node.h>
#include <node_object_wrap.h>

class Mutex : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit Mutex();
  ~Mutex();

  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value> &);
  static void TryLock(const v8::FunctionCallbackInfo<v8::Value> &);
  static void Lock(const v8::FunctionCallbackInfo<v8::Value> &);
  static void Unlock(const v8::FunctionCallbackInfo<v8::Value> &);

  std::mutex mut;
};

class RecursiveMutex : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit RecursiveMutex(double);
  ~RecursiveMutex();

  static void New(const v8::FunctionCallbackInfo<v8::Value> &);
  static void PlusOne(const v8::FunctionCallbackInfo<v8::Value> &);
  static v8::Persistent<v8::Function> constructor;
  double value_;
};

class TimeoutMutex : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit TimeoutMutex(double);
  ~TimeoutMutex();

  static void New(const v8::FunctionCallbackInfo<v8::Value> &);
  static void PlusOne(const v8::FunctionCallbackInfo<v8::Value> &);
  static v8::Persistent<v8::Function> constructor;
  double value_;
};

#endif
