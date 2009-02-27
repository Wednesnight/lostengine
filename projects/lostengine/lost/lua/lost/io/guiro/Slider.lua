module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Slider
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Slider")

function Loader:Slider(definition)
  return lost.guiro.controls.Slider(definition)
end
