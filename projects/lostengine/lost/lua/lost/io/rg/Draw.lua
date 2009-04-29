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

  if definition["active"] then
    result.active = definition["active"]
  end
  
  if definition["material"] then
    local mat = definition["material"]
    for k,v in mat do
      result.material[k] = v
    end
  end
  
  return result
end