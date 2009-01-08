module("lost.guiro.controls", package.seeall)

--[[
     Image control
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Image" (lost.guiro.controls.Control)
Image = _G["lost.guiro.controls.Image"]

lost.guiro.controls.Control:addBase(Image, "Image")

--[[
    constructor
  ]]
function Image:__init() lost.guiro.controls.Control.__init(self)
  self.currentAlpha = 1.0
  self.currentColor = lost.common.Color(1,1,1)
  self.currentFilter = gl.GL_LINEAR
  self.currentStretch = false
end

function Image:alpha(alpha)
  if (alpha ~= nil) and self.currentAlpha ~= alpha then
    self.currentAlpha = alpha
    self:needsRedraw()
  end
  return self.currentAlpha
end

function Image:color(color)
  if (color ~= nil) and self.currentColor ~= color then
    self.currentColor = lost.common.Color(color)
    self:needsRedraw()
  end
  return self.currentColor
end

function Image:filter(filter)
  if (filter ~= nil) and self.currentFilter ~= filter then
    self.currentFilter = filter
    self:needsRedraw()
  end
  return self.currentFilter
end

function Image:stretch(stretch)
  if (stretch ~= nil) and self.currentStretch ~= stretch then
    self.currentStretch = stretch
    self:needsRedraw()
  end
  return self.currentStretch
end
