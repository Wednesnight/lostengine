module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Label Style
  ]]
Label = lost.common.Class("lost.guiro.themes.default.styles.Label", lost.guiro.Style)

function Label:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}
end
