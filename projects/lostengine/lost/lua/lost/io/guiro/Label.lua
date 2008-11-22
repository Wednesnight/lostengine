module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Label
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Label")

function Loader:Label(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/Label.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/Label.lua")
  end

  return self:applyDefinitionToView(lost.guiro.controls.Image(), definition)
end
