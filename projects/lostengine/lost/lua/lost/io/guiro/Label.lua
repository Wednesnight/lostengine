module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Label
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Label")

function Loader:Label(definition)
  return lost.guiro.controls.Label(definition)
end
