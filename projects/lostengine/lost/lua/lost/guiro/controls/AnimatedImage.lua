module("lost.guiro.controls", package.seeall)

require("lost.guiro.controls.Image")

--[[
     AnimatedImage control
  ]]
AnimatedImage = lost.common.Class("lost.guiro.controls.AnimatedImage", lost.guiro.controls.Image)

--[[
    constructor
  ]]
function AnimatedImage:__init(properties) lost.guiro.controls.Image.__init(self, properties)
  properties = properties or {}

  self.currentStep = 0
  self.frameSize = properties.frameSize or lost.math.Rect()
  self.interval = properties.interval or 0.5
end

function AnimatedImage:update(canvas)
  local oldStep = self.currentStep
  if self.lastTimestamp == nil or lost.platform.currentTimeSeconds() - self.lastTimestamp >= self.interval then
    self.lastTimestamp = lost.platform.currentTimeSeconds()
    self.currentStep = self.currentStep + 1
    if self.currentStep == self.bitmap.width / self.frameSize.width then
      self.currentStep = 0
    end
  end
  if oldStep ~= self.currentStep then
    self:needsRedraw()
  end
  lost.guiro.View.update(self, canvas)
end

function AnimatedImage:render(canvas, force)
  lost.guiro.View.render(self, canvas, force)
end
