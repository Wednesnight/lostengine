-- lost.io.rg.Camera.lua
module("lost.io", package.seeall)

require("lost.io.Loader")
-- excepects a table with the following key-value pairs
-- viewport = Rect() with viewport definition: x,y,w,h
function Loader:Camera2D(definition)
  local vp = definition["viewport"]
  if not vp then
    error("viewport parameter is required for Camera2D")
  end  
  local cam = lost.camera.Camera2D.create(vp)
  local result = lost.rg.Camera.create(cam)
  if definition["name"] then
    result.name = definition["name"]
  end
  if definition["active"] then
    result.active = definition["active"]
  end  
  return result
end

-- excepects a table with the following key-value pairs
-- viewport = Rect() with viewport definition: x,y,w,h (mandatory)
-- all other params are optional
-- position = Vec3()
-- direction = Vec3()
-- rotation = Vec3()
-- fovY = float
-- depth = Vec2() (these are the GL znear/zFar) values
-- stickToTarget = bool (true if camera should automatically be repointed at target)
function Loader:Camera3D(definition)
  local vp = definition["viewport"]
  if not vp then
    error("viewport parameter is required for Camera3D")
  end
  local cam = lost.camera.Camera3D.create(vp) -- viewport is mandatory for creation
  local result = lost.rg.Camera.create(cam)
  if definition["position"] then
    cam:position(definition["position"])
  end
  if definition["direction"] then
    cam:direction(definition["direction"])
  end
  if definition["rotation"] then
    cam:rotation(definition["rotation"])
  end
  if definition["fovY"] then
    cam:fovY(definition["fovY"])
  end
  if definition["depth"] then
    cam:depth(definition["depth"])
  end
  if definition["stickToTarget"] then
    cam:stickToTarget(definition["stickToTarget"])
  end
  if definition["name"] then
    result.name = definition["name"]
  end
  if definition["active"] then
    result.active = definition["active"]
  end
  return result
end