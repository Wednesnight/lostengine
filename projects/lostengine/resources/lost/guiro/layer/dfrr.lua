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
  self:filled(filled)
  self:frameWidth(width)
  self:posOffset(Vec2(0,0))
  self:sizeOffset(Vec2(0,0))
  self:topVisible(true)
  self:bottomVisible(true)
  self:leftVisible(true)
  self:rightVisible(true)
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

function dfrr:updateDisplay()
  lost.guiro.layer.Layer.updateDisplay(self)
  local so = lost.math.Vec2()
  local po = lost.math.Vec2()
  local d = 2
  if (not self._topVisible) and (not self._bottomVisible) then
    so.y = so.y + 2*self._frameWidth+2*self._radius+2*d
  elseif (not self._topVisible) or (not self._bottomVisible) then
    so.y = so.y + self._frameWidth+self._radius+d
  end

  if (not self._topVisible) and (not self._bottomVisible) then
    -- stays at 0/0
  elseif not self._topVisible then
    po.y = po.y + so.y/2
  elseif not self._bottomVisible then
    po.y = po.y - so.y/2
  end  
  
  self:sizeOffset(so)
  self:posOffset(po)
end

function dfrr:frameWidth(...)
  if arg.n >= 1 then
    self._frameWidth = arg[1]
    self.mesh.material.uniforms:setFloat("width", math.max(1,self._frameWidth)) -- never let width run under 1 or we'll get artifacts
    self:needsDisplay() -- to adjust size/pos offset for switched off sides
  else
    return self._frameWidth
  end
end

function dfrr:radius(...)
  if arg.n >= 1 then
    self._radius = arg[1]
    self.mesh.material.uniforms:setFloat("radius", self._radius)
    self:needsDisplay()
  else
    return self._radius
  end
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

function dfrr:topVisible(...)
  if arg.n >= 1 then 
    self._topVisible = arg[1]
    self:needsDisplay()
  else
    return self._topVisible
  end
end

function dfrr:bottomVisible(...)
  if arg.n >= 1 then 
    self._bottomVisible = arg[1]
    self:needsDisplay()
  else
    return self._bottomVisible
  end
end

function dfrr:leftVisible(...)
  if arg.n >= 1 then 
    self._leftVisible = arg[1]
    self:needsDisplay()
  else
    return self._leftVisible
  end
end

function dfrr:rightVisible(...)
  if arg.n >= 1 then 
    self._rightVisible = arg[1]
    self:needsDisplay()
  else
    return self._rightVisible
  end
end

function dfrr:filled(...)
  if arg.n >= 1 then 
    self._filled = arg[1]
    local v = 0
    if self._filled then
      v = 1
    end
    log.debug("setting filled to "..tostring(v))
    self.mesh.material.uniforms:setFloat("filled", v)
  else
    return self._filled
  end
end
  
-- Vec2
function dfrr:sizeOffset(...)
  if arg.n >= 1 then 
    self._sizeOffset = arg[1]
    self.mesh.material.uniforms:set("sizeOffset", self._sizeOffset)
  else
    return self._sizeOffset
  end
end

-- Vec2
function dfrr:posOffset(...)
  if arg.n >= 1 then 
    self._posOffset = arg[1]
    self.mesh.material.uniforms:set("posOffset", self._posOffset)
  else
    return self._posOffset
  end
end
  