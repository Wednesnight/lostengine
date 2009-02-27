module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     AnimatedImage Style
  ]]
AnimatedImage = lost.common.Class("lost.guiro.themes.default.styles.AnimatedImage", lost.guiro.Style)

function AnimatedImage:__init(properties) lost.guiro.Style.__init(self, properties)
  properties = properties or {}

  self.borderColor = properties.borderColor or lost.common.Color(0,0,0,0)
end
