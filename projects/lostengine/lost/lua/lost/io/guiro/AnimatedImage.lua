module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: AnimatedImage
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.AnimatedImage")

function Loader:AnimatedImage(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/AnimatedImage.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/AnimatedImage.lua")
  end

  return self:applyDefinitionToView(lost.guiro.controls.AnimatedImage(), definition)
end
