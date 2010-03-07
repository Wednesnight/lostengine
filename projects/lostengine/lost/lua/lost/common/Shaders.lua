module("lost.common.Shaders", package.seeall)

local _colorShader = nil; 
local _textureShader = nil; 

function colorShader()
  if _colorShader == nil then
    _colorShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/basic_color")
  end
  return _colorShader
end

function textureShader()
  if _textureShader == nil then
    _textureShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/basic_texture")
  end
  return _textureShader
end