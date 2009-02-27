module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Style
  ]]

require("lost.io.Loader")
require("lost.guiro.Style")

function Loader:Style(definition)
  return lost.guiro.Style(definition)
end
