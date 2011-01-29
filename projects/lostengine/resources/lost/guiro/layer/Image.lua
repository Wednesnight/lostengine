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
  
  
  -- trigger initial updates
  self:needsUpdate()
  self:needsLayout()
end

function Image:update()
  local my = self.texture.dataHeight / self.texture.height
  local mx = self.texture.dataWidth / self.texture.width
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
  if self._filter then
    self.texture:filter(gl.GL_LINEAR)
  else
    self.texture:filter(gl.GL_NEAREST)
  end
end

function Image:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.mesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))
  if self._scale == "aspect" then -- aspect scales the image to fit the current rect
    local f = math.min(self.rect.width / self.texture.dataWidth, self.rect.height / self.texture.dataHeight)
    local w = math.floor(self.rect.width*f)
    local h = math.floor(self.rect.height*f)
    self.quad:update(Rect(0,0,w, h))    
  else
    self.quad:update(Rect(0,0,self.texture.dataWidth, self.texture.dataHeight)) -- for scale == none
  end
end

function Image:bitmap(v)
  local texparams = lost.gl.Texture.Params()
  self:texture(lost.gl.Texture.create(v, texparams))
end

function Image:texture(v)
  self.texture = v
  self.mesh.material:setTexture(0,self.texture)
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

