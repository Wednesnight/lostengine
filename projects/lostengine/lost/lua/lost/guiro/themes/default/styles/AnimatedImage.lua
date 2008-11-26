module("lost.guiro.themes.default.styles", package.seeall)

--[[
     AnimatedImage Style
  ]]
require("lost.guiro.themes.Style")

class "lost.guiro.themes.default.styles.AnimatedImage" (lost.guiro.themes.Style)
AnimatedImage = _G["lost.guiro.themes.default.styles.AnimatedImage"]

function AnimatedImage:__init() super()
  self.borderColor = lost.common.Color(0,0,0,0)
end
