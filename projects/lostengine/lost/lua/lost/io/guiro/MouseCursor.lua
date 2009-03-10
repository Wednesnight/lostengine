module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Button
  ]]

require("lost.io.Loader")
require("lost.guiro.MouseCursor")

function Loader:MouseCursor(definition)
  return lost.guiro.MouseCursor(definition)
end
