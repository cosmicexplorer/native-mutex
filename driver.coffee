NativeMutex = require './build/Release/native-mutex'

lock = new NativeMutex.Mutex recursive: yes, timed: yes

counter = 0
lock.lock()
++counter
lock.unlock()
console.log ++counter

a = lock.doWithLock true, ->
  console.log "hey"
  3

console.log a

# setInterval((->
#   setTimeout((->
#     process.exit() if counter > 1000
#     console.log ++counter), 1)), 1)
