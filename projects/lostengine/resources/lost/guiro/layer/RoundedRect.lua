module("lost.guiro.layer", package.seeall)

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

  if t.gradientVertical ~= nil then 
    self.gradientVertical = t.gradientVertical
  else
    self.gradientVertical = true
  end

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
  
  if hasGradient then
    if self.gradientVertical then 
      self.mesh.material.shader = lost.guiro.shaderFactory():dfrrGradientV()
    else
      self.mesh.material.shader = lost.guiro.shaderFactory():dfrrGradientH()
    end
  else
    self.mesh.material.shader = lost.guiro.shaderFactory():dfrr()
  end
  local _sides = t.sides or {}
  local _roundCorners = t.roundCorners or {}
  self:sides(_sides)
  self:roundCorners(_roundCorners)
  self:radius(radius)  
  self:filled(filled)
  self:frameWidth(width)
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

function RoundedRect:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
  self.mesh.material.uniforms:set("size", Vec2(self.rect.width, self.rect.height))
end

function RoundedRect:updateDisplay()
  lost.guiro.layer.Layer.updateDisplay(self)
  local so = lost.math.Vec2()
  local po = lost.math.Vec2()
  local d = 2
  if (not self._topVisible) and (not self._bottomVisible) then
    so.y = so.y + 2*self._frameWidth+2*self._radius+2*d
  elseif (not self._topVisible) or (not self._bottomVisible) then
    so.y = so.y + self._frameWidth+self._radius+d
  end

  if (not self._leftVisible) and (not self._rightVisible) then
    so.x = so.x + 2*self._frameWidth+2*self._radius+2*d
  elseif (not self._leftVisible) or (not self._rightVisible) then
    so.x = so.x + self._frameWidth+self._radius+d
  end

  if (not self._topVisible) and (not self._bottomVisible) then
    -- stays at 0/0
  elseif not self._topVisible then
    po.y = po.y + so.y/2
  elseif not self._bottomVisible then
    po.y = po.y - so.y/2
  end  

  if (not self._leftVisible) and (not self._rightVisible) then
    -- stays at 0/0
  elseif not self._rightVisible then
    po.x = po.x + so.x/2
  elseif not self._leftVisible then
    po.x = po.x - so.x/2
  end  
  self:sizeOffset(so)
  self:posOffset(po)
end

function RoundedRect:frameWidth(...)
  if arg.n >= 1 then
    self._frameWidth = arg[1]
    self.mesh.material.uniforms:setFloat("width", math.max(1,self._frameWidth)) -- never let width run under 1 or we'll get artifacts
    self:needsDisplay() -- to adjust size/pos offset for switched off sides
  else
    return self._frameWidth
  end
end

function RoundedRect:radius(...)
  if arg.n >= 1 then
    self._radius = arg[1]
    self.mesh.material.uniforms:setFloat("radius", self._radius)
    self:needsDisplay()
  else
    return self._radius
  end
end

function RoundedRect:color(...)
  if arg.n >= 1 then
    self._color = arg[1]
    self.mesh.material.color = self._color:premultiplied()
  else
    return self._color
  end
end

function RoundedRect:blround(...)
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

function RoundedRect:brround(...)
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

function RoundedRect:tlround(...)
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

function RoundedRect:trround(...)
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

function RoundedRect:topVisible(...)
  if arg.n >= 1 then 
    self._topVisible = arg[1]
    self:needsDisplay()
  else
    return self._topVisible
  end
end

function RoundedRect:bottomVisible(...)
  if arg.n >= 1 then 
    self._bottomVisible = arg[1]
    self:needsDisplay()
  else
    return self._bottomVisible
  end
end

function RoundedRect:leftVisible(...)
  if arg.n >= 1 then 
    self._leftVisible = arg[1]
    self:needsDisplay()
  else
    return self._leftVisible
  end
end

function RoundedRect:rightVisible(...)
  if arg.n >= 1 then 
    self._rightVisible = arg[1]
    self:needsDisplay()
  else
    return self._rightVisible
  end
end

function RoundedRect:filled(...)
  if arg.n >= 1 then 
    self._filled = arg[1]
    local v = 0
    if self._filled then
      v = 1
    end
--    log.debug("setting filled to "..tostring(v))
    self.mesh.material.uniforms:setFloat("filled", v)
  else
    return self._filled
  end
end
  
-- Vec2
function RoundedRect:sizeOffset(...)
  if arg.n >= 1 then 
    self._sizeOffset = arg[1]
    self.mesh.material.uniforms:set("sizeOffset", self._sizeOffset)
  else
    return self._sizeOffset
  end
end

-- Vec2
function RoundedRect:posOffset(...)
  if arg.n >= 1 then 
    self._posOffset = arg[1]
    self.mesh.material.uniforms:set("posOffset", self._posOffset)
  else
    return self._posOffset
  end
end
  
function RoundedRect:sides(t)
local s = {}
if t.top ~= nil then s.top = t.top else s.top = true end
if t.bottom ~= nil then s.bottom = t.bottom else s.bottom = true end
if t.left ~= nil then s.left = t.left else s.left = true end
if t.right ~= nil then s.right = t.right else s.right = true end

  for k,v in pairs(s) do
    if k == "top" then self:topVisible(v) 
    elseif k == "bottom" then self:bottomVisible(v) 
    elseif k == "left" then self:leftVisible(v) 
    elseif k == "right" then self:rightVisible(v) end
  end
end

function RoundedRect:roundCorners(t)
  local rc = {}
if t.tl ~= nil then rc.tl = t.tl else rc.tl = true end
if t.tr ~= nil then rc.tr = t.tr else rc.tr = true end
if t.bl ~= nil then rc.bl = t.bl else rc.bl = true end
if t.br ~= nil then rc.br = t.br else rc.br = true end

  for k,v in pairs(rc) do
    if k == "tl" then self:tlround(v)
    elseif k == "tr" then self:trround(v)
    elseif k == "bl" then self:blround(v)
    elseif k == "br" then self:brround(v) end
  end
end
