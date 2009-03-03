module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.Object")

--[[
     Style
  ]]
Style = lost.common.Class("lost.guiro.Style", lost.common.Object)

function Style:__init(properties) lost.common.Object.__init(self)
  properties = properties or {}

  -- background color
  self.backgroundColor = properties.backgroundColor
  
  -- background image
  self.backgroundImage = properties.backgroundImage
  self.backgroundImageBounds = properties.backgroundImageBounds
  self.backgroundImageFilter = properties.backgroundImageFilter
  if properties.backgroundImageStretched ~= nil then self.backgroundImageStretched = properties.backgroundImageStretched
    else self.backgroundImageStretched = true end
    
  -- border color
  self.borderColor = properties.borderColor
  
  -- round corners
  properties.corners = properties.corners or {}
  self.corners =  properties.corners
  self.corners.size = properties.corners.size
  self.corners.steps = properties.corners.steps
end
