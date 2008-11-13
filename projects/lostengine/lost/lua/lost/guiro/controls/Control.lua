module("lost.guiro.controls", package.seeall)

--[[
     Control class
  ]]
require("lost.guiro.View")

class "lost.guiro.controls.Control" (lost.guiro.View)
Control = _G["lost.guiro.controls.Control"]

lost.guiro.View:addBase(Control, "Control")

function Control:__init() super()
end
