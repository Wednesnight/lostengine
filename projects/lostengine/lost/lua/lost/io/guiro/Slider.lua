module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Slider
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Slider")

function Loader:Slider(definition)
  if not definition.style then
    definition.style = self:StyleFromFile("lost/guiro/styles/default/Slider.lua")
  end
  if not definition.renderer then
    definition.renderer = self:RendererFromFile("lost/guiro/renderer/default/Slider.lua")
  end

  local value = nil
  if (definition.value) then
    value = definition.value
    definition.value = nil
  end
  local slider = self:applyDefinitionToView(lost.guiro.controls.Slider(), definition)
  if value then
    slider:value(value)
  end
  return slider
end
