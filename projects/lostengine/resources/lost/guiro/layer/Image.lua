module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Image" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local MatrixTranslation = lost.math.MatrixTranslation

function Image:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}
  if t.bitmap then -- must be premultiplied alpha
    local texparams = lost.gl.Texture.Params()
    self.texture = lost.gl.Texture.create(t.bitmap, texparams)
  elseif t.texture then
    self.texture = t.texture
  end
  self.quad = lost.guiro.Quad()
  self.mesh = self.quad.mesh
  self.mesh.material:blendPremultiplied()
  self.mesh.material:setTexture(0,self.texture)
  self.mesh.material.shader = lost.guiro.shaderFactory():texture()
  
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)  
  log.debug("---------- tex "..self.texture.dataWidth.." "..self.texture.dataHeight)
  self.quad:update(Rect(0,0,self.texture.dataWidth, self.texture.dataHeight))
  self:needsLayout()
end

function Image:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.mesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))
end
