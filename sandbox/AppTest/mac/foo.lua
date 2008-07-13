local debug = _G.log.debug

module("foo")

function printIt(s)
  debug("from foo: "..s)
end