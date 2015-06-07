native-mutex
============

A node wrapper over C++11 mutexes.

# USAGE

```
var NativeMutex = require('native-mutex');

var lock = new NativeMutex.Mutex();

lock.lock();
// do whatever by yourself
lock.unlock(); // let others have fun
```

# OPERATIONS

The type of mutex is specified in a options object passed to the constructor, as detailed [below](#API). All mutexes support the `lock`, `tryLock`, and `unlock` operations, which work [just like C++](http://www.cplusplus.com/reference/mutex/mutex/). Recursive mutexes support recursive locking (incredible, right) and timeout mutexes support the `tryLockFor` function, which works as detailed [here](http://www.cplusplus.com/reference/mutex/timed_mutex/try_lock_for/), except you pass in a number of milliseconds instead of a C++ std::whatever. All the mutexes are destroyed on garbage collection.

In addition to these, there is a `doWithLock` operation that releases a lock when the function returns. Its use is described below:

```
var result = lock.doWithLock(function(){
  // do something
  return "result"; // return whatever
});
// now the variable result equals "result"
```

It is super super important to note that `doWithLock` runs **synchronously**, which is why it can return a result. Don't forget that, even though it takes a callback like a lot of asynchronous functions. The return value of `doWithLock` is the return value of the callback it runs.

# API

These are described as if they are C++ functions, with argument and return types. They are javascript functions, but it is easier to think of them that way.

- `Mutex(opts)` (constructor)

If 'opts' is a javascript object, checks two properties: `recursive` and `timed`. Allows recursive locking if `recursive` is `true`, and timed locking if `timed` is `true`.

- `void lock(void)`

Locks the mutex. Throws TypeError if already locked and mutex is not recursive. This blocks the calling thread synchronously until it is locked, so try to avoid deadlocks!

- `void unlock(void)`

Unlocks the mutex. Throws TypeError if you don't have the lock.
- `bool tryLock(void)`

Attempts to lock the mutex. If succeeds, returns true. If fails (mutex is already locked by another thread), returns false immediately without locking.

- `bool tryLockFor(int milliseconds)`

Only available for timed mutexes. Attempts to lock the mutex within the given time span. Returns true on success, false on timeout. Throws TypeError if already locked and mutex is not recursive, or if the mutex isn't a timed mutex. This blocks the calling thread synchronously until it locks or times out.

- `object doWithLock((optional) lockItForMe, callbackToRun)`

Runs callback **synchronously**, unlocking mutex when done and returning the result of the callback. If lockItForMe is provided and is `false`, it assumes the mutex is already locked. Otherwise, the mutex is locked for you. When using this function, make sure you don't accidentally lock the mutex before running this without setting lockitForMe to false, or forget to lock the mutex and set lockitForMe to false (although this will throw an exception, so it's not that bad), because you will experience errors, and errors are bad.

# HUH?

## Isn't javascript asynchronous without explicit threading? Why do we need mutexes?

That's true, but sometimes explicit locking is unavoidable. Check out [the test code](driver.coffee) to see an example of how to produce a race condition. If the lock wasn't there, the counter could have been incremented incorrectly, and you'll see that `make check` will likely fail if you remove the lock.

# THIS CODE LOOKS AWFUL

True. Ideally there'd be a lot more inheritance to conform to principles of good software engineering. However, I'm not too familiar with v8 or its C++ API and it seemed easier to do it this way. If there are any bugs in the the approximately five lines of code in this repo, and you actually use this, definitely contact me.
