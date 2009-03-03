module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.Object")

--[[
     Renderer
  ]]
Renderer = lost.common.Class("lost.guiro.Renderer", lost.common.Object)

function Renderer:__init(properties) lost.common.Object.__init(self)
  properties = properties or {}
end

--[[
    render
  ]]
function Renderer:render(canvas, view, style)
  -- draw background with image
  if style.backgroundImage then
    -- initialize background color
    if not style.backgroundColor then
      style.backgroundColor = lost.common.Color(1,1,1)
    end

    -- initialize backgroundTexture
    if not self.backgroundTexture then
      self.backgroundTextureParams = lost.gl.Texture.Params()
      self.backgroundTexture = lost.gl.Texture()
      self.backgroundTexture:init(style.backgroundImage, self.backgroundTextureParams)
    end
    self.backgroundTexture:bind()
    if style.backgroundImageFilter then
      self.backgroundTexture:filter(style.backgroundImageFilter)
    end

    -- valid image corner bounds
    if style.backgroundImageBounds then
      -- draw background image and preserve corners
      self:renderBackgroundImagePreserveCorners(canvas, view, style)
    else
      -- draw background image
      self:renderBackgroundImage(canvas, view, style)
    end
  else
    -- valid background color
    if style.backgroundColor then
      -- draw background filled
      self:renderBackgroundFilled(canvas, view, style)
    end
  end

  -- valid border color
  if style.borderColor then
    -- draw background outline
    self:renderBackgroundOutline(canvas, view, style)
  end
end

function Renderer:renderBackgroundOutline(canvas, view, style)
  local globalRect = view:globalRect()

  canvas:setColor(style.borderColor)
  if style.corners.size then
    if style.corners.steps then
      canvas:drawRectOutlineRounded(globalRect, style.corners.size, style.corners.steps)
    else
      canvas:drawRectOutlineRounded(globalRect, style.corners.size, 25)
    end
  else
    canvas:drawRectOutline(globalRect)
  end
end

function Renderer:renderBackgroundFilled(canvas, view, style)
  local globalRect = view:globalRect()

  canvas:setColor(style.backgroundColor)
  if style.corners.size then
    if style.corners.steps then
      canvas:drawRectFilledRounded(globalRect, style.corners.size, style.corners.steps)
    else
      canvas:drawRectFilledRounded(globalRect, style.corners.size, 25)
    end
  else
    canvas:drawRectFilled(globalRect)
  end
end

function Renderer:renderBackgroundImage(canvas, view, style)
  local globalRect = view:globalRect()
  local imageRect = lost.math.Rect(globalRect)

  canvas:setColor(style.backgroundColor)
  if not style.backgroundImageStretched then
    local ratio = lost.math.Vec2(globalRect.width/style.backgroundImage.width, globalRect.height/style.backgroundImage.height)
    if (ratio.x <= ratio.y) then
      imageRect.width  = math.min(imageRect.width, style.backgroundImage.width * ratio.x)
      imageRect.height = math.min(imageRect.height, style.backgroundImage.height * ratio.x)
    else
      imageRect.width  = math.min(imageRect.width, style.backgroundImage.width * ratio.y)
      imageRect.height = math.min(imageRect.height, style.backgroundImage.height * ratio.y)
    end
  end
  canvas:drawRectTextured(imageRect, self.backgroundTexture, true)
end

function Renderer:renderBackgroundImagePreserveCorners(canvas, view, style)
  local globalRect = view:globalRect()

  canvas:setColor(style.backgroundColor)
  if not self.backgroundMesh then
    self.backgroundMesh = lost.gl.Mesh2D(16, 16, 54, self.backgroundTexture)
  end

  --[[
      calculate raster:
         0  1  2  3
         4  5  6  7
         8  9 10 11
        12 13 14 15
    ]]
  local topLeft = lost.math.Rect(globalRect.x,
                                 globalRect:maxY() - style.backgroundImageBounds.top,
                                 style.backgroundImageBounds.left,
                                 style.backgroundImageBounds.top)
  local middleLeft = lost.math.Rect(globalRect.x,
                                    globalRect.y + style.backgroundImageBounds.bottom,
                                    style.backgroundImageBounds.left,
                                    (globalRect.height - 1 - style.backgroundImageBounds.top) - style.backgroundImageBounds.bottom)
  local bottomLeft = lost.math.Rect(globalRect.x,
                                    globalRect.y,
                                    style.backgroundImageBounds.left,
                                    style.backgroundImageBounds.bottom)
  local topCenter = lost.math.Rect(globalRect.x + style.backgroundImageBounds.left,
                                   globalRect:maxY() - style.backgroundImageBounds.top,
                                   (globalRect.width - 1 - style.backgroundImageBounds.left) - style.backgroundImageBounds.right,
                                   style.backgroundImageBounds.top)
  local middleCenter = lost.math.Rect(globalRect.x + style.backgroundImageBounds.left,
                                      globalRect.y + style.backgroundImageBounds.bottom,
                                      (globalRect.width - 1 - style.backgroundImageBounds.left) - style.backgroundImageBounds.right,
                                      (globalRect.height - 1 - style.backgroundImageBounds.top) - style.backgroundImageBounds.bottom)
  local bottomCenter = lost.math.Rect(globalRect.x + style.backgroundImageBounds.left,
                                      globalRect.y,
                                      (globalRect.width - 1 - style.backgroundImageBounds.left) - style.backgroundImageBounds.right,
                                      style.backgroundImageBounds.bottom)
  local topRight = lost.math.Rect(globalRect:maxX() - style.backgroundImageBounds.right,
                                  globalRect:maxY() - style.backgroundImageBounds.top,
                                  style.backgroundImageBounds.right,
                                  style.backgroundImageBounds.top)
  local middleRight = lost.math.Rect(globalRect:maxX() - style.backgroundImageBounds.right,
                                     globalRect.y + style.backgroundImageBounds.bottom,
                                     style.backgroundImageBounds.right,
                                     (globalRect.height - 1 - style.backgroundImageBounds.top) - style.backgroundImageBounds.bottom)
  local bottomRight = lost.math.Rect(globalRect:maxX() - style.backgroundImageBounds.right,
                                     globalRect.y,
                                     style.backgroundImageBounds.right,
                                     style.backgroundImageBounds.bottom)

  --[[
      set vertices
    ]]
  self.backgroundMesh:setVertex(0, lost.math.Vec2(topLeft.x - 0.5, topLeft.y + topLeft.height + 0.5))
  self.backgroundMesh:setVertex(1, lost.math.Vec2(topLeft.x + topLeft.width + 0.5, topLeft.y + topLeft.height + 0.5))
  self.backgroundMesh:setVertex(2, lost.math.Vec2(topCenter.x + topCenter.width + 0.5, topCenter.y + topCenter.height + 0.5))
  self.backgroundMesh:setVertex(3, lost.math.Vec2(topRight.x + topRight.width + 0.5, topRight.y + topRight.height + 0.5))

  self.backgroundMesh:setVertex(4, lost.math.Vec2(topLeft.x - 0.5, topLeft.y - 0.5))
  self.backgroundMesh:setVertex(5, lost.math.Vec2(topCenter.x - 0.5, topCenter.y - 0.5))
  self.backgroundMesh:setVertex(6, lost.math.Vec2(topRight.x - 0.5, topRight.y - 0.5))
  self.backgroundMesh:setVertex(7, lost.math.Vec2(topRight.x + topRight.width + 0.5, topRight.y - 0.5))

  self.backgroundMesh:setVertex(8, lost.math.Vec2(middleLeft.x - 0.5, middleLeft.y - 0.5))
  self.backgroundMesh:setVertex(9, lost.math.Vec2(middleCenter.x - 0.5, middleCenter.y - 0.5))
  self.backgroundMesh:setVertex(10, lost.math.Vec2(middleRight.x - 0.5, middleRight.y - 0.5))
  self.backgroundMesh:setVertex(11, lost.math.Vec2(middleRight.x + middleRight.width + 0.5, middleRight.y - 0.5))

  self.backgroundMesh:setVertex(12, lost.math.Vec2(bottomLeft.x - 0.5, bottomLeft.y - 0.5))
  self.backgroundMesh:setVertex(13, lost.math.Vec2(bottomCenter.x - 0.5, bottomCenter.y - 0.5))
  self.backgroundMesh:setVertex(14, lost.math.Vec2(bottomRight.x - 0.5, bottomRight.y - 0.5))
  self.backgroundMesh:setVertex(15, lost.math.Vec2(bottomRight.x + bottomRight.width + 0.5, bottomRight.y - 0.5))

  --[[
      set faces
    ]]
  local vertex = 0
  local idx = 0
  for idx = 0, 53, 18 do
    local colIdx = idx
    for col = 1, 3, 1 do
      self.backgroundMesh:setFace(colIdx, vertex) self.backgroundMesh:setFace(colIdx+1, vertex+4) self.backgroundMesh:setFace(colIdx+2, vertex+5)
      self.backgroundMesh:setFace(colIdx+3, vertex) self.backgroundMesh:setFace(colIdx+4, vertex+1) self.backgroundMesh:setFace(colIdx+5, vertex+5)
      vertex = vertex + 1
      colIdx = colIdx + 6
    end
    vertex = vertex + 1
  end

  --[[
      set texcoords
    ]]
  self.backgroundMesh:setTexcoord(0, lost.math.Vec2(0, 0))
  self.backgroundMesh:setTexcoord(1, lost.math.Vec2(topLeft.width / style.backgroundImage.width, 0))
  self.backgroundMesh:setTexcoord(2, lost.math.Vec2(1 - topRight.width / style.backgroundImage.width, 0))
  self.backgroundMesh:setTexcoord(3, lost.math.Vec2(1, 0))

  self.backgroundMesh:setTexcoord(4, lost.math.Vec2(0, style.backgroundImageBounds.top / style.backgroundImage.height))
  self.backgroundMesh:setTexcoord(5, lost.math.Vec2(topLeft.width / style.backgroundImage.width, style.backgroundImageBounds.top / style.backgroundImage.height))
  self.backgroundMesh:setTexcoord(6, lost.math.Vec2(1 - topRight.width / style.backgroundImage.width, style.backgroundImageBounds.top / style.backgroundImage.height))
  self.backgroundMesh:setTexcoord(7, lost.math.Vec2(1, style.backgroundImageBounds.top / style.backgroundImage.height))

  self.backgroundMesh:setTexcoord(8, lost.math.Vec2(0, 1 - style.backgroundImageBounds.bottom / style.backgroundImage.height))
  self.backgroundMesh:setTexcoord(9, lost.math.Vec2(topLeft.width / style.backgroundImage.width, 1 - style.backgroundImageBounds.bottom / style.backgroundImage.height))
  self.backgroundMesh:setTexcoord(10, lost.math.Vec2(1 - topRight.width / style.backgroundImage.width, 1 - style.backgroundImageBounds.bottom / style.backgroundImage.height))
  self.backgroundMesh:setTexcoord(11, lost.math.Vec2(1, 1 - style.backgroundImageBounds.bottom / style.backgroundImage.height))

  self.backgroundMesh:setTexcoord(12, lost.math.Vec2(0, 1))
  self.backgroundMesh:setTexcoord(13, lost.math.Vec2(topLeft.width / style.backgroundImage.width, 1))
  self.backgroundMesh:setTexcoord(14, lost.math.Vec2(1 - topRight.width / style.backgroundImage.width, 1))
  self.backgroundMesh:setTexcoord(15, lost.math.Vec2(1, 1))

  canvas:drawMesh2D(self.backgroundMesh, gl.GL_TRIANGLES)
end
