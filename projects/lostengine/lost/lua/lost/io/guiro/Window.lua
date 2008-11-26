module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Window
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Window")

function Loader:Window(definition)
  return self:applyDefinitionToView(lost.guiro.controls.Window(), definition)
end
