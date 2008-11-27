module("lost.guiro.themes.default.styles", package.seeall)

--[[
     Button Style
  ]]
require("lost.guiro.Style")

class "lost.guiro.themes.default.styles.Button" (lost.guiro.Style)
Button = _G["lost.guiro.themes.default.styles.Button"]

function Button:__init() super()
  self.released =
  {
    color = lost.common.Color(0.5,0.5,0.5,1),
    fontColor = lost.common.Color(1,1,1,1),
    lineColor = lost.common.Color(0,0,0,0)
  }
  self.hovered =
  {
    color = lost.common.Color(0.6,0.6,0.6,1),
    fontColor = lost.common.Color(1,1,1,1),
    lineColor = lost.common.Color(0,0,0,0)
  }
  self.pressed =
  {
    color = lost.common.Color(0.3,0.3,0.3,1),
    fontColor = lost.common.Color(1,1,1,1),
    lineColor = lost.common.Color(0,0,0,0)
  }
end
