-- lost.io.gl.Texture
module("lost.io", package.seeall)

require("lost.io.Loader")

function Loader:TextureParams(definition)
  local result = lost.gl.Texture.Params()
  
  for k,v in pairs(definition) do
    result[k] = v
  end
  
  return result
end

function Loader:Texture(definition)
  local filename = definition["filename"]
  local bitmap = definition["bitmap"]
  local params = definition["params"] or lost.gl.Texture.Params()
  
  local result = nil
  
  if filename ~= nil then
    result = lost.gl.Texture(self.loader:load(filename), params)
  elseif bitmap ~= nil then
    result = lost.gl.Texture(bitmap, params)
  else
    error("filename or bitmap required for constructionof Texture")
  end
  
  return result
end