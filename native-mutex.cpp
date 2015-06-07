#include <node.h>
#include "mutex.hpp"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Mutex::Init(exports);
  RecursiveMutex::Init(exports);
  TimeoutMutex::Init(exports);
}

NODE_MODULE(addon, InitAll);
