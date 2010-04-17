module("lost.common.Shaders", package.seeall)

local _colorShader = nil; 
local _textureShader = nil; 
local _gradientShader = nil; 

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
      _textureShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/es/basic_texture")      
    else
      _textureShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/basic_texture")
    end
  end
  return _textureShader
end

function gradientShader()
  if _gradientShader == nil then
    if lost.platform.isIPhone() then
      _gradientShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/es/gradient")      
    else
      _gradientShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/gradient")
    end
  end
  return _gradientShader
end

function lightShader()
  if _lightShader == nil then
    if lost.platform.isIPhone() then
      _lightShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/es/basic_light")      
    else
      _lightShader = lost.gl.loadShader(lost.application.currentTasklet.loader, "lost/resources/glsl/basic_light")
    end
  end
  return _lightShader
end
