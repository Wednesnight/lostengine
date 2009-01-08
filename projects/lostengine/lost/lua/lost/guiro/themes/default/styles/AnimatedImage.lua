module("lost.guiro.themes.default.styles", package.seeall)

--[[
     AnimatedImage Style
  ]]
require("lost.guiro.Style")

class "lost.guiro.themes.default.styles.AnimatedImage" (lost.guiro.Style)
AnimatedImage = _G["lost.guiro.themes.default.styles.AnimatedImage"]

function AnimatedImage:__init() lost.guiro.Style.__init(self)
  self.borderColor = lost.common.Color(0,0,0,0)
end
