module("lost.guiro.layer", package.seeall)

require("lost.guiro.Bounds")
require("lost.guiro.Quad")
require("lost.guiro.TextureManager")

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
    self._width = t.width
  else
    if self.filled then
      self._width = 0
    else
      self._width = 1
    end
  end

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

  if self.filled then
    if self.gradientName then
      if self.gradientVertical then 
        self.mesh.material.shader = lost.guiro.shaderFactory():rectGradientV()
      else
        self.mesh.material.shader = lost.guiro.shaderFactory():rectGradientH()
      end
    else
      self.mesh.material.shader = lost.guiro.shaderFactory():rect()
    end
  else
    if self.gradientName then
      if self.gradientVertical then 
        self.mesh.material.shader = lost.guiro.shaderFactory():rectFrameGradientV()    
      else
        self.mesh.material.shader = lost.guiro.shaderFactory():rectFrameGradientH()    
      end
    else
      self.mesh.material.shader = lost.guiro.shaderFactory():rectFrame()    
    end
    self.mesh.material.uniforms:setFloat("width", self._width)
  end

  if t.color then
    self.mesh.material.color = t.color:premultiplied()
  else
    lost.common.Color(1,1,1)
  end

  if self.gradientName then
    self.mesh.material:setTexture(0,lost.guiro.textureManager()._textureManager.gradientTexture)
    self.mesh.material.uniforms:setFloat("gradientCoord", self.gradientCoord)
  end
  
  self.drawNode = lost.rg.Draw.create(self.mesh)
  self.layerNodes:add(self.drawNode)
  self:needsLayout()

	self._bbcounter = lost.profiler.BBCount("lost.guiro.layer.Rect")  
end

function Rect:color(...)
  if arg.n >= 1 then 
    self._color = arg[1]
    self.mesh.material.color = self._color:premultiplied()
  else
    return self._color
  end
end

function Rect:updateLayout()
  lost.guiro.layer.Layer.updateLayout(self)
  self.quad:update(self.rect)
  if not self.filled then
    self.mesh.material.uniforms:set("size", Vec2(self.rect.width, self.rect.height))
  end
end