module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Window Style
  ]]
Window = lost.common.Class("lost.guiro.themes.default.styles.Window", lost.guiro.Style)

function Window:__init() lost.guiro.Style.__init(self)
  self.color = lost.common.Color(0.2,0.2,0.2)
  self.lineColor = lost.common.Color(0.5,0.5,0.5)
  self.header =
  {
    color = lost.common.Color(0.1,0.1,0.1),
    fontColor = lost.common.Color(1,1,1)
  }
end
