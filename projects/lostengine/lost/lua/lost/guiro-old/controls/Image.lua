module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")

--[[
     Image control
  ]]
lost.common.Class "lost.guiro.controls.Image" "lost.guiro.View" {}

--[[
    constructor
  ]]
function Image:create(properties)
  properties = properties or {}

  -- initialize defaults
  properties.backgroundImageFilter = properties.backgroundImageFilter or gl.GL_LINEAR

  lost.guiro.View.create(self, properties)
end
