module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Window
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Window")

function Loader:Window(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/Window.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/Window.lua")
  end

  return self:applyDefinitionToView(lost.guiro.controls.Window(), definition)
end
