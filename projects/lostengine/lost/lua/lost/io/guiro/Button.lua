module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Button
  ]]

require("lost.io.Loader")
require("lost.guiro.controls.Button")

function Loader:Button(definition)
  return lost.guiro.controls.Button(definition)
end
