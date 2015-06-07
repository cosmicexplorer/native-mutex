spawn = require('child_process').spawn

NativeMutex = require './build/Release/native-mutex'

lockObj = new NativeMutex.Mutex()

counter = 0

# without the locks, this is a race condition and should eventually fail
doStuff = ->
  succeeded = lockObj.tryLock()
  while not succeeded
    succeeded = lockObj.tryLock()
  e = spawn('echo', [(++counter).toString()])
  e.stdout.on 'data', (dat) ->
    console.log counter
    lockObj.unlock()

# for some reason node doesn't like while(true)
setInterval doStuff, 0

setTimeout((-> process.exit()), 5000)
