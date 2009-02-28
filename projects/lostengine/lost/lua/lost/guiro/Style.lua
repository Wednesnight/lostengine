module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.Object")

--[[
     Style
  ]]
Style = lost.common.Class("lost.guiro.Style", lost.common.Object)

function Style:__init(properties) lost.common.Object.__init(self)
  properties = properties or {}

  self.backgroundColor = properties.backgroundColor
  self.borderColor = properties.borderColor
  properties.corners = properties.corners or {}
  self.corners =  properties.corners
  self.corners.size = properties.corners.size
  self.corners.steps = properties.corners.steps
end
