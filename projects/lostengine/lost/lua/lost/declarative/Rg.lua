-- lost.declarative.Rg
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Rg" {}

function Rg:constructor(loader)
  self.loader = loader
end

function Rg:applyNodeAttributes(targetNode, def)
  local active = def["active"]
  local name = def["name"]
  if active ~= nil then
    targetNode.active = active
  end
  if name ~= nil then
    targetNode.name = name
  end  
end

-- calls the glEnable/glDisable command with GL_BLEND
function Rg:Blend(def)
  local enable = def["enable"]
  if enable == nil then
    error("enable is required for rg:Blend")
  end
  local result = lost.rg.Blend.create(enable)
  
  self:applyNodeAttributes(result, def)  
  return result
end

-- excepects a table with the following key-value pairs
-- viewport = Rect() with viewport def: x,y,w,h
function Rg:Camera2D(def)
  local vp = def["viewport"]
  if not vp then
    error("viewport parameter is required for Camera2D")
  end  
  local cam = lost.camera.Camera2D.create(vp)
  local result = lost.rg.Camera.create(cam)

  self:applyNodeAttributes(result, def)  
  return result
end

-- excepects a table with the following key-value pairs
-- viewport = Rect() with viewport def: x,y,w,h (mandatory)
-- all other params are optional
-- position = Vec3()
-- direction = Vec3()
-- rotation = Vec3()
-- fovY = float
-- depth = Vec2() (these are the GL znear/zFar) values
-- stickToTarget = bool (true if camera should automatically be repointed at target)
function Rg:Camera3D(def)
  local vp = def["viewport"]
  if vp == nil then
    error("viewport parameter is required for Camera3D")
  end
  local cam = lost.camera.Camera3D.create(vp) -- viewport is mandatory for creation
  local result = lost.rg.Camera.create(cam)
  if def["fovY"] ~= nil then
    cam:fovY(def["fovY"])
  end
  if def["depth"] ~= nil then
    cam:depth(def["depth"])
  end
  if def["position"] ~= nil then
    cam:position(def["position"])
  end
  if def["target"] ~= nil then
    cam:target(def["target"])
  end
  if def["stickToTarget"] ~= nil then
    cam:stickToTarget(def["stickToTarget"])
  end

  self:applyNodeAttributes(result, def)  

  return result
end

-- calls the glClear command with the given mask
function Rg:Clear(def)
  local mask = def["mask"]
  if mask == nil then
    error("mask is required for rg:Clear")
  end
  local result = lost.rg.Clear.create(def["mask"])
  
  self:applyNodeAttributes(result, def)  

  return result
end

-- calls the glClear command with the given mask
function Rg:ClearColor(def)
  local color = def["color"]
  if color == nil then
    error("color is required for rg:ClearColor")
  end
  local result = lost.rg.ClearColor.create(color)

  self:applyNodeAttributes(result, def)  

  return result
end

-- calls the glClear command with the given mask
function Rg:DefaultFrameBuffer(def)
  local result = lost.rg.DefaultFrameBuffer.create()
  
  self:applyNodeAttributes(result, def)  

  return result
end

function Rg:DepthTest(def)
  local result = lost.rg.DepthTest.create(def[1])
  
  self:applyNodeAttributes(result, def)  
  
  return result
end

-- mesh  (mandatory)
-- material
function Rg:Draw(def)
  local mesh = def["mesh"]
  if mesh == nil then
    error("mesh is required for rg:Draw")    
  end
  local result = lost.rg.Draw.create(mesh)
  
  self:applyNodeAttributes(result, def)  
  
  return result
end

-- calls the glClear command with the given mask
function Rg:FrameBuffer(def)
  local framebuffer = def["framebuffer"]
  if framebuffer == nil then
    error("framebuffer is required for rg:FrameBuffer")
  end
  local result = lost.rg.FrameBuffer.create(framebuffer)
  
  self:applyNodeAttributes(result, def)  
    
  return result
end

-- node expects an array of other nodes, simple or specialised,
-- which are simply added to the new node
function Rg:Node(def)
  log.debug("loading node")
  local result = lost.rg.Node.create()
  
  for k,v in pairs(def) do
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
