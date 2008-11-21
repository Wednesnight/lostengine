module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Button
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Button")

function Loader:Button(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/Button.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/Button.lua")
  end

  return self:applyDefinitionToView(lost.guiro.controls.Button(), definition)
end
