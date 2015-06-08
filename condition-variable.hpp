#ifndef CONDITION_VARIABLE_HPP
#define CONDITION_VARIABLE_HPP

/* std includes */
#include <mutex>
#include <condition_variable>

/* node includes */
#include <node.h>
#include <node_object_wrap.h>

class ConditionVariable : public node::ObjectWrap {
public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit ConditionVariable();
  ~ConditionVariable();

  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value> &);
  static void Wait(const v8::FunctionCallbackInfo<v8::Value> &);
  static void WaitFor(const v8::FunctionCallbackInfo<v8::Value> &);
  static void NotifyOne(const v8::FunctionCallbackInfo<v8::Value> &);
  static void NotifyAll(const v8::FunctionCallbackInfo<v8::Value> &);

  std::mutex mut;
  std::condition_variable cv;
};

#endif /* CONDITION_VARIABLE_HPP */
