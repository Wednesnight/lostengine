module("lost.guiro.controls", package.seeall)

--[[
     AnimatedImage control
  ]]
require("lost.guiro.controls.Image")

class "lost.guiro.controls.AnimatedImage" (lost.guiro.controls.Image)
AnimatedImage = _G["lost.guiro.controls.AnimatedImage"]

lost.guiro.controls.Image:addBase(AnimatedImage, "AnimatedImage")

--[[
    constructor
  ]]
function AnimatedImage:__init() super()
  self.frameSize = lost.math.Rect()
  self.interval = 0.5
end
