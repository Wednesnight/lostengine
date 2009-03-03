module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")

--[[
     Image control
  ]]
Image = lost.common.Class("lost.guiro.controls.Image", lost.guiro.View)

--[[
    constructor
  ]]
function Image:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}
end
