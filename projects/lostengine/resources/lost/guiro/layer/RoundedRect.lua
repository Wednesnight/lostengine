module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.RoundedRect" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

function RoundedRect:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self.quad = lost.guiro.Quad{}
  self.mesh = self.quad.mesh
  self.mesh.material.shader = ui.shaderFactory:color()
  self.mesh.material.color = t.color or lost.common.Color(1,1,1)
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)
  self:needsLayout()
end

function RoundedRect:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
end
