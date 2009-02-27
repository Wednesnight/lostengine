module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Window Style
  ]]
Window = lost.common.Class("lost.guiro.themes.default.styles.Window", lost.guiro.Style)

function Window:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}

  self.color = properties.color or lost.common.Color(0.2,0.2,0.2)
  self.lineColor = properties.lineColor or lost.common.Color(0.5,0.5,0.5)
  self.header = properties.header or
  {
    color = lost.common.Color(0.1,0.1,0.1),
  }
end
