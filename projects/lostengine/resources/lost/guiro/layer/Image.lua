module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Image" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

function Image:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self:needsLayout()
end

function Image:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
end
