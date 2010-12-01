module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Rect" "lost.guiro.layer.Layer" {}

function Rect:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self.quad = lost.guiro.Quad{}
  self.mesh = self.quad.mesh
  self.mesh.material.shader = ui.shaderFactory:color()
  self.mesh.material.color = t.color or lost.common.Color(1,1,1)
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.renderNode:add(self.drawNode)
  self:needsLayout()
end

function Rect:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
end