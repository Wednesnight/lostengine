module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Slider
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Slider")

function Loader:Slider(definition)
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
