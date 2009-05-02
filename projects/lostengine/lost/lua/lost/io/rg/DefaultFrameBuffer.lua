module("lost.io", package.seeall)

require("lost.io.Loader")

-- calls the glClear command with the given mask
function Loader:DefaultFrameBuffer(definition)
  local result = lost.rg.DefaultFrameBuffer.create()
  
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