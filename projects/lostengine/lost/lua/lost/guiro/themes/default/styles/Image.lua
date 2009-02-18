module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Image Style
  ]]
Image = lost.common.Class("lost.guiro.themes.default.styles.Image", lost.guiro.Style)

function Image:__init() lost.guiro.Style.__init(self)
  self.borderColor = lost.common.Color(0,0,0,0)
end
