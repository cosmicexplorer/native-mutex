byline = require 'byline'
counter = 1
instream = byline(process.stdin)
numNonEmptyLines = 0
instream.on 'data', (line) ->
  if line is ""
    return
  ++numNonEmptyLines
  if parseInt(line) isnt counter
    console.error "RACE CONDITION FOUND: MUTEX FAILED"
    process.exit 1
  ++counter

instream.on 'end', ->
  if numNonEmptyLines is 0
    console.error "NO LINE OUTPUT: TEST IS WRONG"
    process.exit 1
  console.log "i mean, there's no way to know for sure, but like, this mutex
  thing is probably right"
