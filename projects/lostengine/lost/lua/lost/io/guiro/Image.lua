module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Image
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Image")

function Loader:Image(definition)
  local stretch = nil
  if definition.stretch then
    stretch = definition.stretch
    definition.stretch = nil
  end
  local image = self:applyDefinitionToView(lost.guiro.controls.Image(), definition)
  if stretch ~= nil then
    image:stretch(stretch)
  end
  return image
end
