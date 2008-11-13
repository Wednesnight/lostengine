module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Renderer
  ]]

require("lost.io.Loader")

function Loader:RendererFromFile(filename)
  local result
  local guiro = self
  guiro.Renderer = function(self, table) result = table end
  self:executeScript(filename, {guiro = guiro, lost = lost})
  return result
end
