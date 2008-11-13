module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Screen
  ]]

require("lost.io.Loader")
require("lost.guiro.Screen")

function Loader:loadScreen(filename)
  log.debug("Loader:load()")
  local result = {}
  local guiro = self
  guiro.Screen = function(self, definition) result = self:applyDefinitionToView(lost.guiro.Screen(), definition) end
  self:executeScript(filename, {guiro = guiro, lost = lost})
  return result
end
