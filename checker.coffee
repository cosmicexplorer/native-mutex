byline = require 'byline'
counter = 1
instream = byline(process.stdin)
instream.on 'data', (line) ->
  if parseInt(line) isnt counter
    console.error "RACE CONDITION FOUND: MUTEX FAILED"
    process.exit 1
  ++counter

instream.on 'end', ->
  console.log "i mean, there's no way to know for sure, but like, this mutex
  thing is probably right"
