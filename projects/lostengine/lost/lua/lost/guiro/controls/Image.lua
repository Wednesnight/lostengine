module("lost.guiro.controls", package.seeall)

--[[
     Image control
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Image" (lost.guiro.controls.Control)
Image = _G["lost.guiro.controls.Image"]

lost.guiro.controls.Control:addBase(Image, "Image")

--[[
    constructor
  ]]
function Image:__init() super()
  self.alpha = 1.0
end
