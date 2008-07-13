local debug = _G.log.debug

module("baz")

function printIt(s)
  debug("from baz: "..s)
end