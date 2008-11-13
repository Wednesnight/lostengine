module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Renderer
  ]]

require("lost.io.Loader")

function Loader:Renderer(filename)
  local result
  self:executeScript(filename, {guiro = {Renderer = function(self, table) result = table end}, lost = lost})
  return result
end
