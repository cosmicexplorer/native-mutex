NativeMutex = require './build/Release/native-mutex'

lock = new NativeMutex.Mutex recursive: yes, timed: yes

counter = 0
lock.lock()
++counter
lock.unlock()
console.log ++counter

a = lock.doWithLock true, ->
  lock2 = new NativeMutex.Mutex
  lock2.lock()
  lock2.unlock()
  3

console.log a

cv = new NativeMutex.ConditionVariable

ready = no

spawn = require('child_process').spawn
proc = spawn 'curl', ['https://github.com']

proc.on 'exit', ->
  ready = yes
  console.error "!!!" if ready
  cv.notifyOne()

console.error "waiting"
result = cv.waitFor 50, -> ready
while not result
  result = cv.waitFor 50, -> ready
console.error "waited"

# setInterval((->
#   setTimeout((->
#     process.exit() if counter > 1000
#     console.log ++counter), 1)), 1)
