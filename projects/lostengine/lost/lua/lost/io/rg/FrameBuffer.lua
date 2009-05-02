module("lost.io", package.seeall)

require("lost.io.Loader")

-- calls the glClear command with the given mask
function Loader:FrameBuffer(definition)
  local framebuffer = definition["framebuffer"]
  if framebuffer == nil then
    error("framebuffer is required for rg:FrameBuffer")
  end
  local result = lost.rg.FrameBuffer.create(framebuffer)
  
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