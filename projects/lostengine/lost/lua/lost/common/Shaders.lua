module("lost.common.Shaders", package.seeall)

local _colorShader = nil; 
local _textureShader = nil; 

function colorShader()
  if _colorShader == nil then
    if lost.platform.isIPhone() then
      _colorShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/es/basic_color")      
    else
      _colorShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/basic_color")
    end
  end
  return _colorShader
end

function textureShader()
  if _textureShader == nil then
    if lost.platform.isIPhone() then
      _colorShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/es/basic_texture")      
    else
      _textureShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/basic_texture")
    end
  end
  return _textureShader
end