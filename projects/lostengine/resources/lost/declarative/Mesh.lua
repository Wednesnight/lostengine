-- lost.declarative.Mesh

--[[
  common Shaders should be set BEFORE the material is applied so the common Shader can be overwritten with a custom
  one in the material definition.
]]

module("lost.declarative", package.seeall)

require("lost.common.Shaders")

lost.common.Class "lost.declarative.Mesh" {}

function Mesh:constructor(loader)
  self.loader = loader
end

function Mesh:applyMaterial(target, def)
  if def["material"] ~= nil then
    local mat = def["material"]
    for k,v in pairs(mat) do
      target.material[k] = v
    end
	end
end

-- create a line by specifying start/end Vec2
function Mesh:Line(def)
  local pointStart = def["start"]
  local pointEnd = def["finish"]
  
  result = lost.mesh.Line.create(pointStart, pointEnd)
  result.material.shader = lost.common.Shaders.colorShader()

  self:applyMaterial(result, def)
  return result
end

function Mesh:Obj(def)
  local filename = def["filename"] -- path to OBJ file to load
  local file = def["file"] -- handle to OBJ file
  if filename == nil and file == nil then
    error("filename/file is mandatory for mesh:Obj")
  end
  local filedata = file or self.loader:load(filename)
  local objmesh = lost.mesh.Loader.obj(filedata)
  objmesh.material.shader = lost.common.Shaders.colorShader()
  self:applyMaterial(objmesh, def) 
	return objmesh
end

-- create a quad by specifying texture
-- specifying an additional Vec2 size parameter resizes the quad to the given extents
function Mesh:Quad(def)
  local size = def["size"]
  local rect = def["rect"]
  local flip = nil
  if def["flip"] ~= nil then
    flip = def["flip"]
  else
    flip = false
  end
  local filename = def["filename"]
  local tex = def["texture"]
  local result = nil
  local transform = def["transform"]
  
  -- create an instance depending on parameterization
  if filename ~= nil then
    result = lost.mesh.Quad.create(self.loader:load(filename), flip)
    result.material.shader = lost.common.Shaders.textureShader()
  elseif tex ~= nil then 
    result = lost.mesh.Quad.create(tex, flip)
    result.material.shader = lost.common.Shaders.textureShader()
  elseif rect ~= nil then
    result = lost.mesh.Quad.create(rect)
    result.material.shader = lost.common.Shaders.colorShader()
  else
    error("must specify at least one of [filename|tex|rect] for creation of quad")
  end
  if size then
    result:updateSize(size)
  end
  if transform ~= nil then
    result.transform = transform
  end

  self:applyMaterial(result, def)

  return result
end

function Mesh:ScaleGrid(def)
  if def.texture == nil then error("needs texture", 2) end
  if def.rect == nil then error("needs rect",2) end
  if def.left == nil then error("needs left",2) end
  if def.right == nil then error("needs right",2) end
  if def.top == nil then error("needs top",2) end
  if def.bottom == nil then error("needs bottom",2) end
  local flip = false
  if def.flip ~= nil then flip = def.flip end

  local result = lost.mesh.ScaleGrid.create(def.texture, def.rect, def.left, def.right, def.top, def.bottom, flip)
  if def.transform ~= nil then
    result.transform = def.transform
  end
  result.material.shader = lost.common.Shaders.textureShader()
  self:applyMaterial(result, def)

  return result
end

