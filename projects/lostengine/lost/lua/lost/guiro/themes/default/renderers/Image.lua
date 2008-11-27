module("lost.guiro.themes.default.renderers", package.seeall)

--[[
     Image Renderer
  ]]
require("lost.guiro.themes.Renderer")

class "lost.guiro.themes.default.renderers.Image" (lost.guiro.themes.Renderer)
Image = _G["lost.guiro.themes.default.renderers.Image"]

function Image:__init() super()
end

function Image:render(context, image, style)
  local globalRect = image:globalRect()
  
  if not self.texture then
    self.textureParams = lost.gl.Texture.Params()
    self.textureParams.minFilter = image.filter
    self.textureParams.magFilter = image.filter
    self.texture = lost.gl.Texture()
    self.texture:init(image.bitmap, self.textureParams)
  end
  self.texture:bind()
  self.texture:filter(image.filter)

  context:setColor(lost.common.Color(image.color.r, image.color.g, image.color.b, image.alpha))
  if image.cornerBounds then
    if not self.mesh then
      self.mesh = lost.gl.Mesh2D(16, 16, 54, self.texture)
    end

    --[[
        calculate raster:
           0  1  2  3
           4  5  6  7
           8  9 10 11
          12 13 14 15
      ]]
    local topLeft = lost.math.Rect(globalRect.x,
                                   (globalRect.y + globalRect.height - 1) - image.cornerBounds.top,
                                   image.cornerBounds.left,
                                   image.cornerBounds.top)
    local middleLeft = lost.math.Rect(globalRect.x,
                                      globalRect.y + image.cornerBounds.bottom,
                                      image.cornerBounds.left,
                                      (globalRect.height - 1 - image.cornerBounds.top) - image.cornerBounds.bottom)
    local bottomLeft = lost.math.Rect(globalRect.x,
                                      globalRect.y,
                                      image.cornerBounds.left,
                                      image.cornerBounds.bottom)
    local topCenter = lost.math.Rect(globalRect.x + image.cornerBounds.left,
                                     (globalRect.y + globalRect.height - 1) - image.cornerBounds.top,
                                     (globalRect.width - 1 - image.cornerBounds.left) - image.cornerBounds.right,
                                     image.cornerBounds.top)
    local middleCenter = lost.math.Rect(globalRect.x + image.cornerBounds.left,
                                        globalRect.y + image.cornerBounds.bottom,
                                        (globalRect.width - 1 - image.cornerBounds.left) - image.cornerBounds.right,
                                        (globalRect.height - 1 - image.cornerBounds.top) - image.cornerBounds.bottom)
    local bottomCenter = lost.math.Rect(globalRect.x + image.cornerBounds.left,
                                        globalRect.y,
                                        (globalRect.width - 1 - image.cornerBounds.left) - image.cornerBounds.right,
                                        image.cornerBounds.bottom)
    local topRight = lost.math.Rect((globalRect.x + globalRect.width - 1) - image.cornerBounds.right,
                                    (globalRect.y + globalRect.height - 1) - image.cornerBounds.top,
                                    image.cornerBounds.right,
                                    image.cornerBounds.top)
    local middleRight = lost.math.Rect((globalRect.x + globalRect.width - 1) - image.cornerBounds.right,
                                       globalRect.y + image.cornerBounds.bottom,
                                       image.cornerBounds.right,
                                       (globalRect.height - 1 - image.cornerBounds.top) - image.cornerBounds.bottom)
    local bottomRight = lost.math.Rect((globalRect.x + globalRect.width - 1) - image.cornerBounds.right,
                                       globalRect.y,
                                       image.cornerBounds.right,
                                       image.cornerBounds.bottom)

    --[[
        set vertices
      ]]
    self.mesh:setVertex(0, lost.math.Vec2(topLeft.x, topLeft.y + topLeft.height))
    self.mesh:setVertex(1, lost.math.Vec2(topLeft.x + topLeft.width, topLeft.y + topLeft.height))
    self.mesh:setVertex(2, lost.math.Vec2(topCenter.x + topCenter.width, topCenter.y + topCenter.height))
    self.mesh:setVertex(3, lost.math.Vec2(topRight.x + topRight.width, topRight.y + topRight.height))

    self.mesh:setVertex(4, lost.math.Vec2(topLeft.x, topLeft.y))
    self.mesh:setVertex(5, lost.math.Vec2(topCenter.x, topCenter.y))
    self.mesh:setVertex(6, lost.math.Vec2(topRight.x, topRight.y))
    self.mesh:setVertex(7, lost.math.Vec2(topRight.x + topRight.width, topRight.y))

    self.mesh:setVertex(8, lost.math.Vec2(middleLeft.x, middleLeft.y))
    self.mesh:setVertex(9, lost.math.Vec2(middleCenter.x, middleCenter.y))
    self.mesh:setVertex(10, lost.math.Vec2(middleRight.x, middleRight.y))
    self.mesh:setVertex(11, lost.math.Vec2(middleRight.x + middleRight.width, middleRight.y))

    self.mesh:setVertex(12, lost.math.Vec2(bottomLeft.x, bottomLeft.y))
    self.mesh:setVertex(13, lost.math.Vec2(bottomCenter.x, bottomCenter.y))
    self.mesh:setVertex(14, lost.math.Vec2(bottomRight.x, bottomRight.y))
    self.mesh:setVertex(15, lost.math.Vec2(bottomRight.x + bottomRight.width, bottomRight.y))

    --[[
        set faces
      ]]
    local vertex = 0
    local idx = 0
    for idx = 0, 53, 18 do
      local colIdx = idx
      for col = 1, 3, 1 do
        self.mesh:setFace(colIdx, vertex) self.mesh:setFace(colIdx+1, vertex+4) self.mesh:setFace(colIdx+2, vertex+5)
        self.mesh:setFace(colIdx+3, vertex) self.mesh:setFace(colIdx+4, vertex+1) self.mesh:setFace(colIdx+5, vertex+5)
        vertex = vertex + 1
        colIdx = colIdx + 6
      end
      vertex = vertex + 1
    end

    --[[
        set texcoords
      ]]
    self.mesh:setTexcoord(0, lost.math.Vec2(0, 0))
    self.mesh:setTexcoord(1, lost.math.Vec2(topLeft.width / image.bitmap.width, 0))
    self.mesh:setTexcoord(2, lost.math.Vec2(1 - topRight.width / image.bitmap.width, 0))
    self.mesh:setTexcoord(3, lost.math.Vec2(1, 0))

    self.mesh:setTexcoord(4, lost.math.Vec2(0, image.cornerBounds.top / image.bitmap.height))
    self.mesh:setTexcoord(5, lost.math.Vec2(topLeft.width / image.bitmap.width, image.cornerBounds.top / image.bitmap.height))
    self.mesh:setTexcoord(6, lost.math.Vec2(1 - topRight.width / image.bitmap.width, image.cornerBounds.top / image.bitmap.height))
    self.mesh:setTexcoord(7, lost.math.Vec2(1, image.cornerBounds.top / image.bitmap.height))

    self.mesh:setTexcoord(8, lost.math.Vec2(0, 1 - image.cornerBounds.bottom / image.bitmap.height))
    self.mesh:setTexcoord(9, lost.math.Vec2(topLeft.width / image.bitmap.width, 1 - image.cornerBounds.bottom / image.bitmap.height))
    self.mesh:setTexcoord(10, lost.math.Vec2(1 - topRight.width / image.bitmap.width, 1 - image.cornerBounds.bottom / image.bitmap.height))
    self.mesh:setTexcoord(11, lost.math.Vec2(1, 1 - image.cornerBounds.bottom / image.bitmap.height))

    self.mesh:setTexcoord(12, lost.math.Vec2(0, 1))
    self.mesh:setTexcoord(13, lost.math.Vec2(topLeft.width / image.bitmap.width, 1))
    self.mesh:setTexcoord(14, lost.math.Vec2(1 - topRight.width / image.bitmap.width, 1))
    self.mesh:setTexcoord(15, lost.math.Vec2(1, 1))

    context:drawMesh2D(self.mesh, gl.GL_TRIANGLES)

  else
    local imageRect = lost.math.Rect(globalRect)
    if not image.stretch then
      local ratio = lost.math.Vec2(globalRect.width/image.bitmap.width, globalRect.height/image.bitmap.height)
      if (ratio.x <= ratio.y) then
        imageRect.width  = math.min(imageRect.width, image.bitmap.width * ratio.x)
        imageRect.height = math.min(imageRect.height, image.bitmap.height * ratio.x)
      else
        imageRect.width  = math.min(imageRect.width, image.bitmap.width * ratio.y)
        imageRect.height = math.min(imageRect.height, image.bitmap.height * ratio.y)
      end
    end
    context:drawRectTextured(imageRect, self.texture, true)
  end

  context:setColor(style.borderColor)
  context:drawRectOutline(globalRect)
end
