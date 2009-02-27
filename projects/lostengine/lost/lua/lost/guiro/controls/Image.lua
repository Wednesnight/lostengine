module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")

--[[
     Image control
  ]]
Image = lost.common.Class("lost.guiro.controls.Image", lost.guiro.View)

--[[
    constructor
  ]]
function Image:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  self.bitmap = properties.bitmap
  self.cornerBounds = properties.cornerBounds
  self.currentAlpha = properties.alpha or 1.0
  self.currentColor = properties.color or lost.common.Color(1,1,1)
  self.currentFilter = properties.filter or gl.GL_LINEAR
  self.currentStretch = properties.stretch or false
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
