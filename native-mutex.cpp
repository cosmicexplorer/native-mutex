#include <node.h>
#include "mutex.hpp"
#include "condition-variable.hpp"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Mutex::Init(exports);
  ConditionVariable::Init(exports);
}

NODE_MODULE(addon, InitAll);
