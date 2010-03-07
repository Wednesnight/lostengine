-- lost.guiro.Image
module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.common.Shaders")

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local MatrixTranslation = lost.math.MatrixTranslation
local Color = lost.common.Color

lost.common.Class "lost.guiro.Image" "lost.guiro.View" {}

-- render triggers
-- * _bitmap
-- * _texture
-- * _scale

-- layout triggers
-- * _bitmap
-- * _texture
-- * _scale
-- * _caps

--[[  An Image displays one texture inside it's bounds.
]]
function Image:constructor()
  lost.guiro.View.constructor(self)

  self._texture = nil
  self._textureMesh = nil
  self._textureNode = nil
  self._filter = false -- if true, GL_LINEAR is set for min/max filter
  self._flip = false -- if true, texture coordinates are vertically flipped, resulting in an image mirrored at the x axis
  self._scale = "stretch" -- possible values: stretch, aspect, scalegrid
  self._color = Color(1,1,1,1)
  self._caps = -- used for scalegrid
  {
    left = 0,
    right = 0,
    top = 0,
    bottom = 0
  }

  -- show frame by default
  self:showFrame(true)
  -- suppress background by default
  self:showBackground(false)
  
  self.deferredFilterUpdate = function(self, tasklet)
    if self._texture then
      if self._filter then
        --FIXME: binding the texture here is actually a bad idea because it might throw the gl.Context of guard 
        -- and mess up the shadowed state
        self._texture:bind()
        self._texture:filter(gl.GL_LINEAR)
      else
        self._texture:bind()
        self._texture:filter(gl.GL_NEAREST)
      end
    end
  end
  
  self.deferredUpdateColor = function(self, tasklet)
    if self._textureMesh then
      self._textureMesh.material.color = self._color
    end
  end
end

function Image:afterRedraw()
  if not self._texture then
--    log.warn("can't render image beacuse texture is nil")
    return
  end

  if self._scale == "scalegrid" then
    local gr = self.rect
    -- copy global rect since we don't want to modify the view member
    gr = Rect(gr.x, gr.y, gr.width, gr.height)
    -- set to 0 since we move the image through mesh.transform
    gr.x = 0
    gr.y = 0
    self._textureMesh = lost.mesh.ScaleGrid.create(self._texture, gr,
        self._caps.left, self._caps.right, self._caps.top, self._caps.bottom, self._flip)
  else
    -- recreate mesh, we could optimize this to just recreate the texture and attach that to the mesh
    -- but we'll forget that for now
    self._textureMesh = lost.mesh.Quad.create(self._texture, self._flip)
  end
  
  if self._textureMesh then
    self._textureMesh.material.blend = true
    self._textureMesh.material.shader = lost.common.Shaders.textureShader()
  end
  
  -- attach mesh to draw node, remove the node if it's already in the hierarchy
  if self._textureNode ~= nil then
    self.renderNode:remove(self._textureNode)
  end

  -- recreate since setting Node.mesh doesn't have the desired effect
  self._textureNode = lost.rg.Draw.create(self._textureMesh)
  -- inject into draw hierarchy
  self.renderNode:add(self._textureNode)
  self:updateAlign()
end

function Image:afterLayout()
  self:updateAlign()
end

function Image:updateAlign()
  if self._textureMesh then
    -- update mesh bounds and dimension
    if self._scale == "stretch" then
      self._textureMesh:updateSize(Vec2(self.rect.width, self.rect.height))

    elseif self._scale == "aspect" then
      local w, h = self.rect.width / self._texture.width, self.rect.height / self._texture.height
      self._textureMesh:updateSize(Vec2(self._texture.width * math.min(w, h), self._texture.height * math.min(w, h)))

    elseif self._scale == "scalegrid" then
      self._textureMesh:updateSize(Vec2(self.rect.width, self.rect.height),
          self._caps.left, self._caps.right, self._caps.top, self._caps.bottom)
    end
    self._textureMesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))
  end
end

function Image:bitmap(b)
  local texture = lost.gl.Texture.create(b, lost.gl.Texture.Params())
  self:texture(texture)
end

function Image:texture(t)
  if t ~= nil then
    self._texture = t
    self:needsRedraw()
  else
    return self._texture
  end
end

function Image:scale(s)
  if s ~= nil then
    self._scale = s
    self:needsRedraw()
  else
    return self._scale
  end
end

function Image:caps(c)
  if c ~= nil then
    self._caps = c
    self:needsLayout()
  else
    return self._caps
  end
end

function Image:filter(flag)
  if flag ~= nil then
    self._filter = flag
    callLater(self.deferredFilterUpdate, self)
  else
    return self._filter
  end
end

function Image:flip(flag)
  if flag ~= nil then
    self._flip = flag
    self:needsRedraw()
  else
    return self._flip
  end
end

function Image:color(col)
  if col ~= nil then 
    self._color = col
    callLater(self.deferredUpdateColor, self)
  else
    return self._color
  end
end
