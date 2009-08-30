-- lost.guiro.Image
module("lost.guiro", package.seeall)

require("lost.guiro.View")

local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local MatrixTranslation = lost.math.MatrixTranslation

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

  self._scale = "stretch" -- possible values: stretch, aspect, scalegrid

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
end

function Image:render()
  if not self._texture then
    log.warn("can't render image beacuse texture is nil")
    return
  end

  if self._scale == "scalegrid" then
    -- call View directly, otherwise our states get mixed up
    local gr = lost.guiro.View.updateLayout(self, false)
    -- copy global rect since we don't want to modify the view member
    gr = Rect(gr.x, gr.y, gr.width, gr.height)
    -- set to 0 since we move the image through mesh.transform
    gr.x = 0
    gr.y = 0

    self._textureMesh = lost.mesh.ScaleGrid2D.create(self._texture, gr,
        self._caps.left, self._caps.right, self._caps.top, self._caps.bottom)
  else
    -- recreate mesh, we could optimize this to just recreate the texture and attach that to the mesh
    -- but we'll forget that for now
    self._textureMesh = lost.mesh.Quad2D.create(self._texture, true)
  end
  -- attach mesh to draw node, remove the node if it's already in the hierarchy
  if self._textureNode ~= nil then
    self.renderNode:remove(self._textureNode)
  end

  -- recreate since setting Node.mesh doesn't have the desired effect
  self._textureNode = lost.rg.Draw.create(self._textureMesh)
  -- inject into draw hierarchy
  self.renderNode:add(self._textureNode)

  self:updateLayout(false)
end

function Image:updateLayout(forceUpdate)
  local doUpdateLayout = forceUpdate or self.dirtyLayout

  local gr, lr = lost.guiro.View.updateLayout(self, forceUpdate)

  if doUpdateLayout then
    -- update mesh bounds and dimension
    if self._scale == "stretch" then
      self._textureMesh:updateSize(Vec2(gr.width, gr.height), true)

    elseif self._scale == "aspect" then
      local w, h = gr.width / self._texture.width, gr.height / self._texture.height
      self._textureMesh:updateSize(Vec2(self._texture.width * math.min(w, h), self._texture.height * math.min(w, h)), true)

    elseif self._scale == "scalegrid" then
      self._textureMesh:updateSize(Vec2(gr.width, gr.height),
          self._caps.left, self._caps.right, self._caps.top, self._caps.bottom)
    end
    log.debug(tostring(gr))
    self._textureMesh.transform = MatrixTranslation(Vec3(gr.x, gr.y, 0))
  end

  return gr, lr
end

function Image:bitmap(b)
  local texture = lost.gl.Texture(b, lost.gl.Texture.Params())
  self:texture(texture)
end

function Image:texture(t)
  if t ~= nil then
    self._texture = t
    self:render()
  else
    return self._texture
  end
end

function Image:scale(s)
  if s ~= nil then
    self._scale = s
    self:render()
  else
    return self._scale
  end
end

function Image:caps(c)
  if c ~= nil then
    self._caps = c
    self:updateLayout(false)
  else
    return self._caps
  end
end
