module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.HLine")

lost.common.Class "lost.guiro.layer.HLine" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

-- valid params:
-- * all layer params
-- * color: default is (1,1,1,1)
function HLine:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self.hline = lost.guiro.HLine{}
  self.mesh = self.hline.mesh
  self.mesh.material.shader = lost.guiro.shaderFactory():color()  
  if t.color then
    self.mesh.material.color = t.color:premultiplied()
  else
    lost.common.Color(1,1,1)
  end
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)
  self:needsLayout()
end

function HLine:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.hline:update(self.rect)
end
