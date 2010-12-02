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
  
  local sides = t.sides or {}
  local roundCorners = t.roundCorners or {}
  local filled = true
  if t.filled ~= nil then
    filled = t.filled
  end
  local radius = t.radius or 8
  local width = t.width or 1
  
  self.mesh.material.shader = ui.shaderFactory:roundedRect(filled, roundCorners, sides)
  self.mesh.material.uniforms:setFloat("radius", radius)
  if not filled then
    self.mesh.material.uniforms:setFloat("width", width)
  end
  self.mesh.material.color = t.color or lost.common.Color(1,1,1)
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)
  self:needsLayout()
end

function RoundedRect:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
  self.mesh.material.uniforms:set("size", Vec2(self.rect.width, self.rect.height))
end
