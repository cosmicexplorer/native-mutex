#ifndef MUTEX_HPP
#define MUTEX_HPP

/* std includes */
#include <mutex>
#include <stdexcept>

/* node includes */
#include <node.h>
#include <node_object_wrap.h>

class Mutex : public node::ObjectWrap {
public:
  static void Init(v8::Handle<v8::Object> exports);

private:
  explicit Mutex(double);
  ~Mutex();

  static void New(const v8::FunctionCallbackInfo<v8::Value> &);
  static void PlusOne(const v8::FunctionCallbackInfo<v8::Value> &);
  static v8::Persistent<v8::Function> constructor;
  double value_;
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
