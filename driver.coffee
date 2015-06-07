PthreadMutex = require './build/Release/pthread-mutex'

doStuff = ->
  obj = new PthreadMutex.RecursiveMutex()
  console.log obj.plusOne()
  console.log obj.plusOne()

setInterval(doStuff, 500)
