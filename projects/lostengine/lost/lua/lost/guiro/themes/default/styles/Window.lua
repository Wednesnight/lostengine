module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Window Style
  ]]
Window = lost.common.Class("lost.guiro.themes.default.styles.Window", lost.guiro.Style)

function Window:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}

  self.backgroundColor = properties.backgroundColor or lost.common.Color(0.2,0.2,0.2)
  self.borderColor = properties.borderColor or lost.common.Color(0.5,0.5,0.5)
end
