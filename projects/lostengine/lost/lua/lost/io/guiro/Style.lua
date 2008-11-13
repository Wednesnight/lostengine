module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Style
  ]]

require("lost.io.Loader")

function Loader:StyleFromFile(filename)
  local result
  local guiro = self
  guiro.Style = function(self, table) result = table end
  self:executeScript(filename, {guiro = guiro, lost = lost})
  return result
end
