module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.Object")

--[[
     Style
  ]]
Style = lost.common.Class("lost.guiro.Style", lost.common.Object)

function Style:__init(properties) lost.common.Object.__init(self)
  properties = properties or {}
end
