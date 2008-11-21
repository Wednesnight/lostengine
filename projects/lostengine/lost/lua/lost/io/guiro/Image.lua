module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Image
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Image")

function Loader:Image(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/Image.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/Image.lua")
  end

  return self:applyDefinitionToView(lost.guiro.controls.Image(), definition)
end
