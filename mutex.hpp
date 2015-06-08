#ifndef MUTEX_HPP
#define MUTEX_HPP

/* std includes */
#include <mutex>

/* node includes */
#include <node.h>
#include <node_object_wrap.h>

class Mutex : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 public:
  explicit Mutex(bool);
  ~Mutex();

  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value> &);
  static void TryLock(const v8::FunctionCallbackInfo<v8::Value> &);
  static void Lock(const v8::FunctionCallbackInfo<v8::Value> &);
  static void Unlock(const v8::FunctionCallbackInfo<v8::Value> &);
  static void TryLockFor(const v8::FunctionCallbackInfo<v8::Value> &);
  static void DoWithLock(const v8::FunctionCallbackInfo<v8::Value> &);

  static Mutex * GetCorrectMutexType(bool, bool);

  virtual void lock() = 0;
  virtual bool tryLock() = 0;
  virtual bool unlock() = 0;
  virtual void tryLockFor(const v8::FunctionCallbackInfo<v8::Value> & args,
                          v8::Isolate * isolate);
  const bool isTimed;
};

class StandardMutex : public Mutex {
 public:
  explicit StandardMutex();
  ~StandardMutex();

  virtual void lock();
  virtual bool tryLock();
  virtual bool unlock();
  bool isLocked;
  std::mutex mut;
};

class RecursiveMutex : public Mutex {
 public:
  explicit RecursiveMutex();
  ~RecursiveMutex();

  virtual void lock();
  virtual bool tryLock();
  virtual bool unlock();
  size_t numLocks;
  std::recursive_mutex mut;
};

class TimedMutex : public Mutex {
 public:
  explicit TimedMutex();
  ~TimedMutex();

  virtual void lock();
  virtual bool tryLock();
  virtual void tryLockFor(const v8::FunctionCallbackInfo<v8::Value> & args,
                          v8::Isolate * isolate);
  virtual bool tryLockFor(size_t);
  virtual bool unlock();
  bool isLocked;
  std::timed_mutex mut;
};

class RecursiveTimedMutex : public Mutex {
 public:
  explicit RecursiveTimedMutex();
  ~RecursiveTimedMutex();

  virtual void lock();
  virtual bool tryLock();
  virtual void tryLockFor(const v8::FunctionCallbackInfo<v8::Value> & args,
                          v8::Isolate * isolate);
  virtual bool tryLockFor(size_t);
  virtual bool unlock();
  size_t numLocks;
  std::recursive_timed_mutex mut;
};

#endif
