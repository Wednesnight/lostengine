module("lost.guiro.fx", package.seeall)

--[[
     Size Effect
  ]]
require("lost.guiro.fx.Effect")

class "lost.guiro.fx.Size" (lost.guiro.fx.Effect)
Size = _G["lost.guiro.fx.Size"]

function Size:__init(view, params) lost.guiro.fx.Effect.__init(self, view, params)
  if not self.params.factor then
    self.params.factor = 1.0
  end
  if not self.params.steps then
    self.params.steps = 10
  end

  self.params.stepWidth = ((view.bounds.width * self.params.factor) - view.bounds.width) / self.params.steps
  self.params.stepHeight = ((view.bounds.height * self.params.factor) - view.bounds.height) / self.params.steps

  self.currentSize = lost.math.Rect(self.view.bounds)
  self.targetSize = lost.math.Rect(self.view.bounds.x, self.view.bounds.y,
                                   self.view.bounds.width*self.params.factor, self.view.bounds.height*self.params.factor)
end

function Size:renderEffect(context)
  context:pushState(self.renderState)

  if (self.currentSize.width ~= self.targetSize.width) then
    self.currentSize.width = self.currentSize.width + self.params.stepWidth
  end
  if (self.currentSize.height ~= self.targetSize.height) then
    self.currentSize.height = self.currentSize.height + self.params.stepHeight
  end

  context:setColor(lost.common.Color(1,1,1))
  context:drawRectTextured(lost.math.Rect(self.currentSize), self.colorTexture, false)

  if (self.currentSize.width == self.targetSize.width) and (self.currentSize.height == self.targetSize.height) then
    self:finish()
  end

  context:popState()
end
