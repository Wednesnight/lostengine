module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")

lost.common.Class "lost.guiro.layer.Image" "lost.guiro.layer.Layer" {}

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local MatrixTranslation = lost.math.MatrixTranslation
local Color = lost.common.Color

function Image:constructor(args)
  lost.guiro.layer.Layer.constructor(self, args)
  local t = args or {}

  -- mesh and rg
  self.quad = lost.guiro.Quad()
  self.mesh = self.quad.mesh
  self.mesh.material:blendPremultiplied()
  self.mesh.material.shader = lost.guiro.shaderFactory():texture()
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)  

  -- attributes
  if t.bitmap then -- must be premultiplied alpha
    self:bitmap(t.bitmap)
  elseif t.texture then
    self:texture(t.texture)
  end
  self:flip(t.flip or false)
  self:filter(t.filter or false)
  self:scale(t.scale or "none") -- none, aspect  FIXME: needs stretch, repeat
  self:color(t.color or Color(1,1,1))
  self:valign(t.valign or "center")
  self:halign(t.halign or "center")
  -- trigger initial updates
  self:needsUpdate()
  self:needsLayout()

	self._bbcounter = lost.profiler.BBCount("lost.guiro.layer.Image")  
end

function Image:valign(...)
  if arg.n >= 1 then
    self._valign = arg[1]
    self:needsLayout()
  else
    return self._valign
  end
end

function Image:halign(...)
  if arg.n >= 1 then
    self._halign = arg[1]
    self:needsLayout()
  else
    return self._halign
  end
end

function Image:update()
  local my = 0
  local mx = 0

  if self._texture ~= nil then
    my = self._texture.dataHeight / self._texture.height
    mx = self._texture.dataWidth / self._texture.width

    if self._filter then
      self._texture:filter(gl.GL_LINEAR)
    else
      self._texture:filter(gl.GL_NEAREST)
    end
  end

  if self._flip then -- flip image vertically 
    self.mesh:set(0,gl.UT_texcoord0, Vec2(0,my))
    self.mesh:set(1,gl.UT_texcoord0, Vec2(mx,my))
    self.mesh:set(2,gl.UT_texcoord0, Vec2(mx,0))
    self.mesh:set(3,gl.UT_texcoord0, Vec2(0,0))  
  else -- normal texture coords
    self.mesh:set(0,gl.UT_texcoord0, Vec2(0,0))
    self.mesh:set(1,gl.UT_texcoord0, Vec2(mx,0))
    self.mesh:set(2,gl.UT_texcoord0, Vec2(mx,my))
    self.mesh:set(3,gl.UT_texcoord0, Vec2(0,my))
  end
end

function Image:color(...)
  if arg.n >= 1 then
    local c = arg[1]
    self.mesh.material.color = c:premultiplied()
  end
  -- FIXME: return color, needs to take premultiplication into account
end

function Image:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.mesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))
  local x = 0
  local y = 0
  local w = 0
  local h = 0
  if self._scale == "aspect" then -- aspect scales the image to fit the current rect
    if self._texture then
      local f = math.min(self.rect.width / self._texture.dataWidth, self.rect.height / self._texture.dataHeight)
      w = math.floor(self.rect.width*f)
      h = math.floor(self.rect.height*f)        
    end
  else
    if self._texture then
      w = self._texture.dataWidth
      h = self._texture.dataHeight
    end
  end
  if self._valign == "top" then y = self.rect.height - h
  elseif self._valign == "center" then y = math.floor((self.rect.height - h)/2)
  elseif self._valign == "bottom" then y = 0 end

  if self._halign == "left" then x = 0
  elseif self._halign == "center" then x = math.floor((self.rect.width - w)/2)
  elseif self._halign == "right" then x = self.rect.width-w end

  self.quad:update(Rect(x,y,w, h))    
end

function Image:bitmap(v)
  local texparams = lost.gl.Texture.Params()
  self:texture(lost.gl.Texture.create(v, texparams))
end

function Image:texture(v)
  self._texture = v
  self.mesh.material:setTexture(0,self._texture)
  self:needsUpdate()
  self:needsLayout()
end

function Image:flip(...)
  if arg.n >= 1 then
    self._flip = arg[1]
    self:needsUpdate()
  else
    return self._flip
  end
end

function Image:filter(...)
  if arg.n >= 1 then
    self._filter = arg[1]
    self:needsUpdate()
  else
    return self._filter
  end
end

function Image:scale(...)
  if arg.n >= 1 then
    self._scale = arg[1]
    self:needsLayout()
  else
    return self._scale
  end
end

