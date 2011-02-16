module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.common.ShaderFactory" {}

function ShaderFactory:constructor(loader)
  self.loader = loader
  self.cache = {}
end

function ShaderFactory:loadShader(name)
  local shader = self.cache[name]
  if not shader then
    shader = lost.gl.loadShader(self.loader, name)
    self.cache[name] = shader
  end
  return shader
end

function ShaderFactory:ring() return self:loadShader("lost/resources/glsl/ring") end
function ShaderFactory:ringGradientV() return self:loadShader("lost/resources/glsl/ringGradientV") end
function ShaderFactory:ringGradientH() return self:loadShader("lost/resources/glsl/ringGradientH") end
function ShaderFactory:disc() return self:loadShader("lost/resources/glsl/disc") end
function ShaderFactory:discGradientV() return self:loadShader("lost/resources/glsl/discGradientV") end
function ShaderFactory:discGradientH() return self:loadShader("lost/resources/glsl/discGradientH") end
function ShaderFactory:box() return self:loadShader("lost/resources/glsl/box") end
function ShaderFactory:rect() return self:loadShader("lost/resources/glsl/rect") end
function ShaderFactory:rectGradientV() return self:loadShader("lost/resources/glsl/rectGradientV") end
function ShaderFactory:rectGradientH() return self:loadShader("lost/resources/glsl/rectGradientH") end
function ShaderFactory:rectFrame() return self:loadShader("lost/resources/glsl/rectFrame") end
function ShaderFactory:rectFrameGradientV() return self:loadShader("lost/resources/glsl/rectFrameGradientV") end
function ShaderFactory:rectFrameGradientH() return self:loadShader("lost/resources/glsl/rectFrameGradientH") end
function ShaderFactory:dfrr() return self:loadShader("lost/resources/glsl/dfrr") end
function ShaderFactory:dfrrGradientV() return self:loadShader("lost/resources/glsl/dfrrGradientV") end
function ShaderFactory:dfrrGradientH() return self:loadShader("lost/resources/glsl/dfrrGradientH") end
function ShaderFactory:color() return self:loadShader("lost/resources/glsl/basic_color") end
function ShaderFactory:texture() return self:loadShader("lost/resources/glsl/basic_texture") end
