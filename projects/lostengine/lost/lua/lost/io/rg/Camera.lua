module("lost.io", package.seeall)

require("lost.io.Loader")

-- excepects a table with the following key-value pairs
-- viewport = Rect() with viewport definition: x,y,w,h
function Loader:Camera2D(definition)
  local vp = definition["viewport"]
  local result = lost.rg.Camera2D.create(vp)
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
  local result = lost.rg.Camera3D.create(vp) -- viewport is mandatory for creation
  if definition["position"] then
    result:position(definition["position"])
  end
  if definition["direction"] then
    result:direction(definition["direction"])
  end
  if definition["rotation"] then
    result:rotation(definition["rotation"])
  end
  if definition["fovY"] then
    result:fovY(definition["fovY"])
  end
  if definition["depth"] then
    result:depth(definition["depth"])
  end
  if definition["stickToTarget"] then
    result:stickToTarget(definition["stickToTarget"])
  end
  return result
end