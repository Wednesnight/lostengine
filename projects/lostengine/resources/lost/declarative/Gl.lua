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
  -- "filename" is the mandatory basename from which vertex/fragment shader names
  -- are derived for the actual load process
  local filename = def["filename"]
  local source = def["source"]
  if filename == nil and source == nil then
    error("filename/source is required for gl:Shader",2)
  end

  local shaderProgram = lost.gl.ShaderProgram.create()
  local vertexShader = lost.gl.VertexShader.create()
  local fragmentShader = lost.gl.FragmentShader.create()

  local vertexSource = nil
  local fragmentSource = nil

  local libs = def["libs"]
  if libs ~= nil then
    for k,shader in next,libs do
      shaderProgram:attach(shader)
    end
  end

  if filename ~= nil then
    local vsFile = self.loader:load(filename..".vs")
    if vsFile then
      vertexSource = vsFile:str()
    end
    local fsFile = self.loader:load(filename..".fs")
    if fsFile then
      fragmentSource = fsFile:str()
    end
  elseif source ~= nil then
    vertexSource = source.vertexShader
    fragmentSource = source.fragmentShader
  end

  if vertexSource ~= nil then
    vertexShader:source(vertexSource)
    vertexShader:compile()
    if not vertexShader:compiled() then
      error(vertexShader:log(), 2)
    end
    shaderProgram:attach(vertexShader)
  end

  if fragmentSource ~= nil then
    fragmentShader:source(fragmentSource)
    fragmentShader:compile()
    if not fragmentShader:compiled() then
      error(fragmentShader:log(), 2)
    end
    shaderProgram:attach(fragmentShader)
  end

  shaderProgram:link()
  if not shaderProgram:linked() then
    error(shaderProgram:log(), 2)
  end

  shaderProgram:enable()
  shaderProgram:buildUniformMap()
  shaderProgram:buildVertexAttributeMap()

  -- apply params if present
  local params = def["params"]
  if params ~= nil then
    for k,v in pairs(params) do
      local nt = shaderProgram:numericalType(k)
      -- since Lua only knows floats/doubles, we need to disambiguate here by
      -- numerical type of the parameter and call the appropriate setter
      if nt == gl.GL_INT then
        shaderProgram:setInt(k, v)
      elseif nt == gl.GL_FLOAT then
        shaderProgram:setFloat(k,v)
      elseif nt == gl.GL_BOOL then
        shaderProgram:setBool(k,v)
      else
        shaderProgram:set(k,v)
      end
    end
  end

  shaderProgram:disable()

  return shaderProgram
end

function Gl:VertexShader(def)
  -- "filename" is the mandatory
  local filename = def["filename"]
  if filename == nil then
    error("filename is required for gl:VertexShader",2)
  end

  local vertexShader = lost.gl.VertexShader.create()
  local vsFile = self.loader:load(filename)
  vertexShader:source(vsFile:str())
  vertexShader:compile()
  if not vertexShader:compiled() then
    error(vertexShader:log(), 2)
  end
  return vertexShader
end

function Gl:FragmentShader(def)
  -- "filename" is the mandatory
  local filename = def["filename"]
  if filename == nil then
    error("filename is required for gl:FragmentShader",2)
  end

  local fragmentShader = lost.gl.FragmentShader.create()
  local fsFile = self.loader:load(filename)
  fragmentShader:source(fsFile:str())
  fragmentShader:compile()
  if not fragmentShader:compiled() then
    error(fragmentShader:log(), 2)
  end
  return fragmentShader
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
