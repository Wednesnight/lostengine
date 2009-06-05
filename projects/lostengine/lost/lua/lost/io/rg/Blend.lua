module("lost.io", package.seeall)

require("lost.io.Loader")

-- calls the glEnable/glDisable command with GL_BLEND
function Loader:Blend(definition)
  local enable = definition["enable"]
  if enable == nil then
    error("enable is required for rg:Blend")
  end
  local result = lost.rg.Blend.create(enable)
  
  local active = definition["active"]
  local name = definition["name"]
  if active ~= nil then
    result.active = active
  end
  if name ~= nil then
    result.name = name
  end
  return result
end
