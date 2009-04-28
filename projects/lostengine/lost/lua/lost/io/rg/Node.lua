module("lost.io", package.seeall)


require("lost.io.Loader")

-- node expects an array of other nodes, simple or specialised,
-- which are simply added to the new node
function Loader:Node(definition)
  local result = lost.rg.Node.create()
  
  for k,v in pairs(definition) do
    result:add(v)
  end
  
  return result
end