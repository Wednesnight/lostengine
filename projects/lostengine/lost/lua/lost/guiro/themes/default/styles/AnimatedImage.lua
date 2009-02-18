module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     AnimatedImage Style
  ]]
AnimatedImage = lost.common.Class("lost.guiro.themes.default.styles.AnimatedImage", lost.guiro.Style)

function AnimatedImage:__init() lost.guiro.Style.__init(self)
  self.borderColor = lost.common.Color(0,0,0,0)
end
