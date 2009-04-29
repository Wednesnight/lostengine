-- lost.io.rg.Node
module("lost.io", package.seeall)


require("lost.io.Loader")

-- node expects an array of other nodes, simple or specialised,
-- which are simply added to the new node
function Loader:Node(definition)
  log.debug("loading node")
  local result = lost.rg.Node.create()
  
  for k,v in pairs(definition) do
    if type(k) == "string" then
      result[k] = v
    else
      log.debug("adding child node: "..v.name)
      result:add(v)
    end
  end
  
  return result
end