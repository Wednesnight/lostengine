-- lost.io.rg.Draw
module("lost.io", package.seeall)


require("lost.io.Loader")

-- mesh = (any meshPtr, will be cast to MeshPtr) (mandatory)
-- material
function Loader:Draw(definition)
  local result = lost.rg.Draw.create(definition["mesh"])
  
  if definition["modelTransform"] then
    result.modelTransform = definition["modelTransform"]
  end

  if definition["name"] then
    result.name = definition["name"]
  end

  if definition["active"] ~= nil then
    result.active = definition["active"]
  end		
  
  return result
end