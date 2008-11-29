module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Slider
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Slider")

function Loader:Slider(definition)
  local button = nil
  if definition.button then
    button = definition.button
    definition.button = nil
  end
  local value = nil
  if definition.value then
    value = definition.value
    definition.value = nil
  end

  local slider = self:applyDefinitionToView(lost.guiro.controls.Slider(), definition)

  if button then
    slider:setButton(button)
  end
  if value then
    slider:value(value, true)
  end

  return slider
end
