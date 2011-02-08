module("lost.guiro.layer", package.seeall)

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
  self:blround(true)
  self:brround(true)
  self:tlround(true)
  self:trround(true)
  self:radius(radius)  
  if not filled then self.mesh.material.uniforms:setFloat("width", width) end
  if hasGradient then 
    self.mesh.material.uniforms:setFloat("gradientCoord", self.gradientCoord)
    self.mesh.material:setTexture(0,lost.guiro.textureManager()._textureManager.gradientTexture)
  end
  if t.color then
    self:color(t.color)
  else
    self:color(lost.common.Color(1,1,1))
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

function dfrr:width(v)
  if self.mesh.material.shader:hasUniform("width") then
    self.mesh.material.uniforms:setFloat("width", v)
  end
end

function dfrr:radius(v)
  self.mesh.material.uniforms:setFloat("radius", v)
end

function dfrr:color(...)
  if arg.n >= 1 then
    self._color = arg[1]
    self.mesh.material.color = self._color:premultiplied()
  else
    return self._color
  end
end

function dfrr:blround(...)
  if arg.n >= 1 then 
    self._blround = arg[1]
    local v = 1
    if self._blround then
      v = 0
    end
    self.mesh.material.uniforms:setFloat("blrect", v)
  else
    return self._blround
  end
end

function dfrr:brround(...)
  if arg.n >= 1 then 
    self._brround = arg[1]
    local v = 1
    if self._brround then
      v = 0
    end
    self.mesh.material.uniforms:setFloat("brrect", v)
  else
    return self._brround
  end
end

function dfrr:tlround(...)
  if arg.n >= 1 then 
    self._tlround = arg[1]
    local v = 1
    if self._tlround then
      v = 0
    end
    self.mesh.material.uniforms:setFloat("tlrect", v)
  else
    return self._tlround
  end
end

function dfrr:trround(...)
  if arg.n >= 1 then 
    self._trround = arg[1]
    local v = 1
    if self._trround then
      v = 0
    end
    self.mesh.material.uniforms:setFloat("trrect", v)
  else
    return self._trround
  end
end
  