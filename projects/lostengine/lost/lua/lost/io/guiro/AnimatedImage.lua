module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: AnimatedImage
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.AnimatedImage")

function Loader:AnimatedImage(definition)
  return self:applyDefinitionToView(lost.guiro.controls.AnimatedImage(), definition)
end
