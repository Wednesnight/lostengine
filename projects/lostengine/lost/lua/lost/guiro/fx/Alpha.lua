module("lost.guiro.fx", package.seeall)

--[[
     Alpha Effect
  ]]
require("lost.guiro.fx.Effect")

class "lost.guiro.fx.Alpha" (lost.guiro.fx.Effect)
Alpha = _G["lost.guiro.fx.Alpha"]

function Alpha:__init(view, params) lost.guiro.fx.Effect.__init(self, view, params)
  if not self.params.alpha then
    self.params.alpha = 1.0
  end
end

function Alpha:renderEffect(context)
  context:pushState(self.renderState)

  context:setColor(lost.common.Color(1,1,1,self.params.alpha))
  local rect = lost.math.Rect(self.view.bounds)
  context:drawRectTextured(rect, self.colorTexture, false)

  context:popState()
end
