module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Screen
  ]]

require("lost.io.Loader")

function Loader:Screen(definition)
  return self:applyDefinitionToView(globals.app.screen, definition)
end
