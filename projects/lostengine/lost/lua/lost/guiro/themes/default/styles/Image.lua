module("lost.guiro.themes.default.styles", package.seeall)

--[[
     Image Style
  ]]
require("lost.guiro.themes.Style")

class "lost.guiro.themes.default.styles.Image" (lost.guiro.themes.Style)
Image = _G["lost.guiro.themes.default.styles.Image"]

function Image:__init() super()
  self.borderColor = lost.common.Color(0,0,0,0)
end
