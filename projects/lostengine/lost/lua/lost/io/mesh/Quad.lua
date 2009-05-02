-- lost.io.mesh.Quad
module("lost.io", package.seeall)

require("lost.io.Loader")

-- create a quad by specifying texture
-- specifying an additional Vec2 size parameter resizes the quad to the given extents
function Loader:Quad2D(definition)
  local size = definition["size"]
  local rect = definition["rect"]
  local flip = nil
  if definition["flip"] then
    flip = definition["flip"]
  else
    flip = false
  end
  local filename = definition["filename"]
  local tex = definition["texture"]
  local result = nil
  local transform = definition["transform"]
  
  -- create an instance depending on parameterization
  if filename then
    result = lost.mesh.Quad2D(self.loader:load(filename), flip)
  elseif tex then 
    result = lost.mesh.Quad2D(tex, flip)
  elseif rect then
    result = lost.mesh.Quad2D(rect)
  else
    error("must specify at least one of [filename|tex|rect] for creation of quad")
  end
  if size then
    result:updateSize(size, flip)
  end
  if transform ~= nil then
    result.modelTransform = transform
  end

  if definition["material"] ~= nil then
    local mat = definition["material"]
    for k,v in pairs(mat) do
      result.material[k] = v
    end
	end

  return result
end