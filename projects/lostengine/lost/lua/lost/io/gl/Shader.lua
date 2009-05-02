-- lost.io.gl.Shader
module("lost.io", package.seeall)

require("lost.io.Loader")

-- NOTE: this Shader/ShaderParams construct is really a bit misleading since it forces 
-- you to construct and parametrise a shader in one go, which is NOT the common use case.
-- You should check carefully which parameters are static, which params need to be 
-- updated more often and treat them appropriately in your own code.
-- This loader construction really only helps to get you off the ground quickly, but is
-- NOT the most efficient or common construction method.

-- ShaderParams are really only a table containing arbitrary key/value pairs
-- these pairs are evaluated and applied in the Shader load function if they are present
function Loader:ShaderParams(definition)
  return definition
end

function Loader:Shader(definition)
  -- "name" is the mandatory basename from which vertex/fragment shader names
  -- are derived for the actual load process
  local shaderBaseName = definition["filename"]
  if not shaderBaseName then
    error("filename is required for gl:Shader")
  end
  local shaderProgram = lost.gl.loadShader(self.loader, shaderBaseName)

  -- apply params if present
  local params = definition["params"]
  if params ~= nil then
    shaderProgram:enable()
    for k,v in pairs(params) do
      local nt = shaderProgram:numericalType(k)
      -- since Lua only knows floats/doubles, we need to disambiguate here by
      -- numerical type of the paraeter and call the appropriate setter
      if k == gl.GL_INT then
        shaderProgram:setInt(k, v)
      elseif k == gl.GL_FLOAT then
        shaderProgram:setFloat(k,v)
      else
        shaderProgram:set(k,v)
      end
    end
    shaderProgram:disable()
  end
  
  return shaderProgram
end