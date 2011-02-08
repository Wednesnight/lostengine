module("lost.guiro.layer", package.seeall)

--require("lost.guiro.Bounds")
--require("lost.guiro.Quad")
--require("lost.guiro.TextureManager")

lost.common.Class "lost.guiro.layer.dfrr" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2

function dfrr:constructor(args)
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

  if t.gradient ~= nil then
    self.gradientName = t.gradient
  end
  if self.gradientName then
    self.gradientCoord = lost.guiro.textureManager():gradientCoord(self.gradientName)
  end

  local hasGradient = false
  if self.gradientName then
    hasGradient = true
  end
  
  self.mesh.material.shader = lost.guiro.shaderFactory():dfrr()
  if self.mesh.material.shader:hasUniform("radius") then
    self.mesh.material.uniforms:setFloat("radius", radius)
  end
  
  if not filled then self.mesh.material.uniforms:setFloat("width", width) end
  if hasGradient then 
    self.mesh.material.uniforms:setFloat("gradientCoord", self.gradientCoord)
    self.mesh.material:setTexture(0,lost.guiro.textureManager()._textureManager.gradientTexture)
  end
  if t.color then
    self.mesh.material.color = t.color:premultiplied()
  else
    self.mesh.material.color = lost.common.Color(1,1,1)
  end
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)
  self:needsLayout()
end

function dfrr:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
  self.mesh.material.uniforms:set("size", Vec2(self.rect.width, self.rect.height))
end
