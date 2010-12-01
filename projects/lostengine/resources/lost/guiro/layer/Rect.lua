module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Rect" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

-- valid params:
-- * all layer params
-- * color: default is (1,1,1,1)
-- * filled: true/false, default is true. If false, a frame with the specified width will be drawn
-- * width: used if rect is not filled. Default is 0 for not filled, 1 for filled.
function Rect:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  self.quad = lost.guiro.Quad{}
  self.mesh = self.quad.mesh
  if t.filled ~= nil then
    self.filled = t.filled
  else
    self.filled = true
  end
  if t.width ~= nil then
    self.width = t.width
  else
    if self.filled then
      self.width = 0
    else
      self.width = 1
    end
  end
  if self.filled then
    self.mesh.material.shader = ui.shaderFactory:color()
  else
    self.mesh.material.shader = ui.shaderFactory:rectFrame()    
  end
  self.mesh.material.color = t.color or lost.common.Color(1,1,1)
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)
  self:needsLayout()
end

function Rect:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
  if not self.filled then
    self.mesh.material.uniforms:setFloat("width", self.width)
    self.mesh.material.uniforms:set("size", Vec2(self.rect.width, self.rect.height))
  end
end