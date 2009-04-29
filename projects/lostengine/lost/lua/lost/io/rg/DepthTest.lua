module("lost.io", package.seeall)

require("lost.io.Loader")

function Loader:DepthTest(definition)
  local result = lost.rg.DepthTest.create(definition[1])
  
  if definition["active"] then
    result.active = definition["active"]
  end
  
  return result
end