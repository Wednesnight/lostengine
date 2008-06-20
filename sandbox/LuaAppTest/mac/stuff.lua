local pairs = _G.pairs
local print = _G.print

module("stuff")

function printkeys(t)
  for k,v in pairs(t)
  do
    print(k)
  end
end

function Application(t)
	printkeys(t)
end