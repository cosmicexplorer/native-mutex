NativeMutex = require './build/Release/native-mutex'

doStuff = ->
  obj = new NativeMutex.RecursiveMutex()
  console.log obj.plusOne()
  console.log obj.plusOne()

setInterval(doStuff, 500)
