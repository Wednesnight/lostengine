module("lost.guiro.controls", package.seeall)

--[[
     Label control
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Label" (lost.guiro.controls.Control)
Label = _G["lost.guiro.controls.Label"]

lost.guiro.controls.Control:addBase(Label, "Label")

--[[
    constructor
  ]]
function Label:__init() super()
  self.alpha = 1.0
  self.color = lost.common.Color(1,1,1)
end