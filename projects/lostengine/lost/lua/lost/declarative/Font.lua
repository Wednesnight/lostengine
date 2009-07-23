-- lost.declarative.Font
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Font" {}

function Font:create(loader)
  self.loader = loader
end

-- this is essentially an alternative Mesh constructor 
-- so you can apply transform and material just as usual
-- render the given text with the given size and font
-- all three parameters are requireed or an error is raised
function Font:Text(def)
  if (def.text == nil) or
     (def.size == nil) or
     (def.font == nil) then
     error("text, font and size are required",2)
  end   
  local result = def.font:render(def.text, def.size)
  if def.transform ~= nil then
    result.modelTransform = def.transform
  end

  if def["material"] ~= nil then
    local mat = def["material"]
    for k,v in pairs(mat) do
      result.material[k] = v
    end
	end  
  
  return result
end

function Font:TrueTypeFont(def)
  local lib = def.lib or lost.font.freetype.Library.create()
  if def.filename == nil then
    error("filename is required font:TrueTypeFont", 2)
  end
  local data = self.loader:load(def.filename)
  local fnt = lost.font.TrueTypeFont.create(lib, data)
  if def.atlasSize then
    fnt.atlasSize = def.atlasSize
  end
  return fnt
end