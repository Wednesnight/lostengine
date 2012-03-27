module("lost.shaders", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.shaders.ShaderFactory"
{
  GL2GLSLVersion =
  {
    GL2 = 120,
    GL3 = 140,
    GL4 = 150
  }
}

function ShaderFactory:constructor(loader, version)
  self.loader = loader
  self.version = version or "GL2"

  self.shaderCache = {}
end

function ShaderFactory:newInstance(shaderName)
  local instance = nil

  local shader = self:load(shaderName)
  if shader ~= nil then
    instance = lost.gl.ShaderProgram.create()

    local vertexSource = ""
    local fragmentSource = ""

    if shader.include ~= nil then
      for k,lib in next,shader.include do
        if lib.vertex ~= nil then
          local v = lost.gl.VertexShader.create()
--          log.debug("loading vertex: "..lib.vertex.header..lib.vertex.source)
          v:source(lib.vertex.header..lib.vertex.source)
          v:compile()
          if not v:compiled() then
            error(v:log(), 2)
          end
          instance:attach(v)
          if lib.export.vertex ~= nil then
            vertexSource = vertexSource .. lib.export.vertex .. "\n"
          end
        end
        if lib.fragment ~= nil then
          local f = lost.gl.FragmentShader.create()
--          log.debug("loading fragment: "..lib.fragment.header..lib.fragment.source)
          f:source(lib.fragment.header..lib.fragment.source)
          f:compile()
          if not f:compiled() then
            error(f:log(), 2)
          end
          instance:attach(f)
          if lib.export.fragment ~= nil then
            fragmentSource = fragmentSource .. lib.export.fragment .. "\n"
          end
        end
      end
    end

    if shader.vertex ~= nil then
      local vertexShader = lost.gl.VertexShader.create()
--      log.debug("loading vertex: "..shader.vertex.header..vertexSource..shader.vertex.source)
      vertexShader:source(shader.vertex.header..vertexSource..shader.vertex.source)
      vertexShader:compile()
      if not vertexShader:compiled() then
        error(vertexShader:log(), 2)
      end
      instance:attach(vertexShader)
    end

    if shader.fragment ~= nil then
      local fragmentShader = lost.gl.FragmentShader.create()
--      log.debug("loading fragment: "..shader.fragment.header..fragmentSource..shader.fragment.source)
      fragmentShader:source(shader.fragment.header..fragmentSource..shader.fragment.source)
      fragmentShader:compile()
      if not fragmentShader:compiled() then
        error(fragmentShader:log(), 2)
      end
      instance:attach(fragmentShader)
    end

    instance:link()
    if not instance:linked() then
      error(instance:log(), 2)
    end

    instance:enable()
    instance:buildUniformMap()
    instance:buildVertexAttributeMap()
    instance:disable()
  end

  return instance
end

function ShaderFactory:load(shaderName)
  if self.shaderCache[shaderName] == nil then
    local filename = string.gsub(shaderName, "%.", "/")
    if filename ~= nil then
      filename = filename..".glsl"
      local shader = self.loader:load(filename)
      if shader ~= nil then
--        log.debug("loading shader: ".. shader:str())
        local chunk = loadstring(shader:str(), filename)
        setfenv(chunk, {Shader = self})
        self.shaderCache[shaderName] = chunk()
      end
    else
      error(shaderName.." is not a valid shader name", 2)
    end
  end
  return self.shaderCache[shaderName]
end

function ShaderFactory:declare(definition)
  local result = {}
  result.include = definition.include
  result.export = definition.export or {}
  if definition[self.version] then
    result.vertex = definition[self.version].vertex
    result.fragment = definition[self.version].fragment
  else
    result.vertex = definition.vertex
    result.fragment = definition.fragment
  end
  return result
end

function ShaderFactory:source(source)
  local result = {header = "", source = source}

  local debugInfo = debug.getinfo(2)
  if debugInfo ~= nil then
    result.header = "#line " .. debugInfo.currentline .. "\n" .. result.header
  end

  if self.GL2GLSLVersion[self.version] ~= nil then
    result.header = "#version " .. self.GL2GLSLVersion[self.version] .. "\n" .. result.header
  end

  return result
end
