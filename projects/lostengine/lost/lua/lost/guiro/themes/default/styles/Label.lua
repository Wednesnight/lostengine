module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Label Style
  ]]
Label = lost.common.Class("lost.guiro.themes.default.styles.Label", lost.guiro.Style)

function Label:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}

  self.align = properties.align or "center"
  self.color = properties.color or lost.common.Color(1,1,1)
  self.font = properties.font or lost.guiro.config.theme.defaultFont
  self.fontSize = properties.fontSize or 12
end
