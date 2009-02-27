module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Image
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Image")

function Loader:Image(definition)
  return lost.guiro.controls.Image(definition)
end
