module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")

--[[
     Image control
  ]]
Image = lost.common.Class("lost.guiro.controls.Image", lost.guiro.View)

--[[
    constructor
  ]]
function Image:__init(properties)
  properties = properties or {}

  -- initialize defaults
  properties.backgroundImageFilter = properties.backgroundImageFilter or gl.GL_LINEAR

  lost.guiro.View.__init(self, properties)
end
