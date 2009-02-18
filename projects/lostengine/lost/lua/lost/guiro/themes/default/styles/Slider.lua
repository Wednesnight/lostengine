module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Slider Style
  ]]
Slider = lost.common.Class("lost.guiro.themes.default.styles.Slider", lost.guiro.Style)

function Slider:__init() lost.guiro.Style.__init(self)
  self.color = lost.common.Color(0.2,0.2,0.2,1)
  self.lineColor = lost.common.Color(0,0,0,1)
end
