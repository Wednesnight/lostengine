module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: AnimatedImage
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.AnimatedImage")

function Loader:AnimatedImage(definition)
  local stretch = nil
  if definition.stretch then
    stretch = definition.stretch
    definition.stretch = nil
  end
  local image = self:applyDefinitionToView(lost.guiro.controls.AnimatedImage(), definition)
  if stretch ~= nil then
    image:stretch(stretch)
  end
  return image
end
