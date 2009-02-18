module("lost.guiro.controls", package.seeall)

require("lost.guiro.controls.Image")

--[[
     AnimatedImage control
  ]]
AnimatedImage = lost.common.Class("lost.guiro.controls.AnimatedImage", lost.guiro.controls.Image)

--[[
    constructor
  ]]
function AnimatedImage:__init() lost.guiro.controls.Image.__init(self)
  self.currentStep = 0
  self.frameSize = lost.math.Rect()
  self.interval = 0.5
end

function AnimatedImage:update(context)
  local oldStep = self.currentStep
  if self.lastTimestamp == nil or os.clock() - self.lastTimestamp >= self.interval then
    self.lastTimestamp = os.clock()
    self.currentStep = self.currentStep + 1
    if self.currentStep == self.bitmap.width / self.frameSize.width then
      self.currentStep = 0
    end
  end
  if oldStep ~= self.currentStep then
    self:needsRedraw()
  end
  lost.guiro.View.update(self, context)
end

function AnimatedImage:render(context, force)
  lost.guiro.View.render(self, context, force)
end
