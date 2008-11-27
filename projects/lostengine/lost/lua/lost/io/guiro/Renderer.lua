module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Renderer
  ]]

require("lost.io.Loader")
require("lost.guiro.Renderer")

function Loader:Renderer(definition)
  return self:apply(lost.guiro.Renderer(), definition)
end
