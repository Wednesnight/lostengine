module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: AnimatedImage
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.AnimatedImage")

function Loader:AnimatedImage(definition)
  return lost.guiro.controls.AnimatedImage(definition)
end
