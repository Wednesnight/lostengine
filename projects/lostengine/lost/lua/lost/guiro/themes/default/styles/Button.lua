module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Button Style
  ]]
Button = lost.common.Class("lost.guiro.themes.default.styles.Button", lost.guiro.Style)

function Button:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}

  self.released = properties.released or
  {
    color = lost.common.Color(0.5,0.5,0.5,1),
    fontColor = lost.common.Color(1,1,1,1),
    lineColor = lost.common.Color(0,0,0,0)
  }
  self.hovered = properties.hovered or 
  {
    color = lost.common.Color(0.6,0.6,0.6,1),
    fontColor = lost.common.Color(1,1,1,1),
    lineColor = lost.common.Color(0,0,0,0)
  }
  self.pressed = properties.pressed or
  {
    color = lost.common.Color(0.3,0.3,0.3,1),
    fontColor = lost.common.Color(1,1,1,1),
    lineColor = lost.common.Color(0,0,0,0)
  }
end
