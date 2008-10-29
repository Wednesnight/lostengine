module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Style
  ]]

require("lost.io.Loader")

function Loader:Style(filename)
  local result
  self:executeScript(filename, {guiro = {Style = function(table) result = table end}, lost = lost})
  return result
end
