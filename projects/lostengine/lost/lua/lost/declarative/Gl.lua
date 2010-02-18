-- lost.declarative.Gl
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Gl" {}

function Gl:constructor(loader)
  self.loader = loader
end

-- NOTE: this Shader/ShaderParams construct is really a bit misleading since it forces 
-- you to construct and parametrise a shader in one go, which is NOT the common use case.
-- You should check carefully which parameters are static, which params need to be 
-- updated more often and treat them appropriately in your own code.
-- This loader construction really only helps to get you off the ground quickly, but is
-- NOT the most efficient or common construction method.

-- ShaderParams are really only a table containing arbitrary key/value pairs
-- these pairs are evaluated and applied in the Shader load function if they are present
function Gl:ShaderParams(def)
  return def
end

function Gl:Shader(def)
  -- "name" is the mandatory basename from which vertex/fragment shader names
  -- are derived for the actual load process
  local shaderBaseName = def["filename"]
  if not shaderBaseName then
    error("filename is required for gl:Shader",2)
  end
  local shaderProgram = lost.gl.loadShader(self.loader, shaderBaseName)

  -- apply params if present
  local params = def["params"]
  if params ~= nil then
    shaderProgram:enable()
    for k,v in pairs(params) do
      local nt = shaderProgram:numericalType(k)
      -- since Lua only knows floats/doubles, we need to disambiguate here by
      -- numerical type of the parameter and call the appropriate setter
      if nt == gl.GL_INT then
        shaderProgram:setInt(k, v)
      elseif nt == gl.GL_FLOAT then
        shaderProgram:setFloat(k,v)
      else
        shaderProgram:set(k,v)
      end
    end
    shaderProgram:disable()
  end
  
  return shaderProgram
end

-- we can't pass through the actual definition here, because def is a lua table, but
-- the Texture Constructor requires a C++ object. So, we simply instantiate the object here
-- and iterate over all the members of def that are set and copy them over to the target param
-- object
function Gl:TextureParams(def)
  local result = lost.gl.Texture.Params()
  
  for k,v in pairs(def) do
    result[k] = v
  end
  
  return result
end

function Gl:Texture(def)
  local filename = def["filename"]
  local bitmap = def["bitmap"]
  local params = def["params"] or lost.gl.Texture.Params()
  
  if filename and bitmap then
    error("you cannot specify both filename and bitmap for the creation of a texture",2)
  end
  
  local result = nil
  
  if filename ~= nil then
    result = lost.gl.Texture.create(self.loader:load(filename), params)
  elseif bitmap ~= nil then
    result = lost.gl.Texture.create(bitmap, params)
  else
    error("filename or bitmap required for construction of Texture", 2)
  end
  
  return result
end
