module("lost.guiro.themes.default.styles", package.seeall)

--[[
     Slider Style
  ]]
require("lost.guiro.Style")

class "lost.guiro.themes.default.styles.Slider" (lost.guiro.Style)
Slider = _G["lost.guiro.themes.default.styles.Slider"]

function Slider:__init() super()
  self.color = lost.common.Color(0.2,0.2,0.2,1)
  self.lineColor = lost.common.Color(0,0,0,1)
end
