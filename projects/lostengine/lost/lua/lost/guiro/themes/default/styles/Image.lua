module("lost.guiro.themes.default.styles", package.seeall)

require("lost.guiro.Style")

--[[
     Image Style
  ]]
Image = lost.common.Class("lost.guiro.themes.default.styles.Image", lost.guiro.Style)

function Image:__init(properties)
  properties = properties or {}
  properties.backgroundImageFilter = properties.backgroundImageFilter or gl.GL_LINEAR
  
  lost.guiro.Style.__init(self, properties)
end
