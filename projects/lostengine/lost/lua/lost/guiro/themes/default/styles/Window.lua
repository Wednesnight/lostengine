module("lost.guiro.themes.default.styles", package.seeall)

--[[
     Window Style
  ]]
require("lost.guiro.Style")

class "lost.guiro.themes.default.styles.Window" (lost.guiro.Style)
Window = _G["lost.guiro.themes.default.styles.Window"]

function Window:__init() lost.guiro.Style.__init(self)
  self.color = lost.common.Color(0.2,0.2,0.2,1)
  self.lineColor = lost.common.Color(0.5,0.5,0.5,1)
  self.header =
  {
    color = lost.common.Color(0.1,0.1,0.1,1),
    fontColor = lost.common.Color(0.7,0.7,0.7,1),
  }
end
