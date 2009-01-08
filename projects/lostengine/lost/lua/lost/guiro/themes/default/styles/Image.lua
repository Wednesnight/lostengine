module("lost.guiro.themes.default.styles", package.seeall)

--[[
     Image Style
  ]]
require("lost.guiro.Style")

class "lost.guiro.themes.default.styles.Image" (lost.guiro.Style)
Image = _G["lost.guiro.themes.default.styles.Image"]

function Image:__init() lost.guiro.Style.__init(self)
  self.borderColor = lost.common.Color(0,0,0,0)
end
