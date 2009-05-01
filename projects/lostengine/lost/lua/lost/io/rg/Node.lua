-- lost.io.rg.Node
module("lost.io", package.seeall)


require("lost.io.Loader")

-- node expects an array of other nodes, simple or specialised,
-- which are simply added to the new node
function Loader:Node(definition)
  log.debug("loading node")
  local result = lost.rg.Node.create()
  
  for k,v in pairs(definition) do
    -- if the key is a string, we assume that the user wanted to set a node
    -- attribute and we simply assign it without any checks.
    if type(k) == "string" then
      result[k] = v
    else
      -- if the key was not a string we assume it was an integer therefore 
      -- iterating over the array part of the table where the child nodes
      -- should be stored. We store the current value as child node without
      -- any further checks
      log.debug("adding child node: "..v.name)
      result:add(v)
    end
  end
  
  return result
end