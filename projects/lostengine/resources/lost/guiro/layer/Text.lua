module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Text" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

-- valid params:
-- * all layer params
function Text:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
end

function Text:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
end