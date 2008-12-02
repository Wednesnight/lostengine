module("lost.guiro.themes.default.renderers", package.seeall)

--[[
     AnimatedImage Renderer
  ]]
require("lost.guiro.Renderer")

class "lost.guiro.themes.default.renderers.AnimatedImage" (lost.guiro.Renderer)
AnimatedImage = _G["lost.guiro.themes.default.renderers.AnimatedImage"]

function AnimatedImage:__init() super()
end

function AnimatedImage:render(context, image, style)
  local globalRect = image:globalRect()

  if not self.texture then
    self.textureParams = lost.gl.Texture.Params()
    self.textureParams.minFilter = image:filter()
    self.textureParams.magFilter = image:filter()
    self.texture = lost.gl.Texture()
    self.texture:init(image.bitmap, self.textureParams)
  end
  self.texture:bind()
  self.texture:filter(image:filter())

  local imageColor = image:color()
  context:setColor(lost.common.Color(imageColor.r, imageColor.g, imageColor.b, imageColor.a * image:alpha()))
  local imageRect = lost.math.Rect(globalRect)
  if not image:stretch() then
    local ratio = lost.math.Vec2(globalRect.width/image.frameSize.width, globalRect.height/image.frameSize.height)
    if (ratio.x <= ratio.y) then
      imageRect.width  = math.min(imageRect.width, image.frameSize.width * ratio.x)
      imageRect.height = math.min(imageRect.height, image.frameSize.height * ratio.x)
    else
      imageRect.width  = math.min(imageRect.width, image.frameSize.width * ratio.y)
      imageRect.height = math.min(imageRect.height, image.frameSize.height * ratio.y)
    end
  end

  local tmpRect = lost.math.Rect(image.frameSize)
  tmpRect.x = tmpRect.width * image.currentStep
  local bottomLeft = lost.math.Vec2(tmpRect.x/image.bitmap.width, 0)
  local bottomRight = lost.math.Vec2((tmpRect.x + tmpRect.width)/image.bitmap.width, 0)
  local topLeft = lost.math.Vec2(tmpRect.x/image.bitmap.width, 1)
  local topRight = lost.math.Vec2((tmpRect.x + tmpRect.width)/image.bitmap.width, 1)
  context:drawRectTextured(imageRect, self.texture, bottomLeft, bottomRight, topLeft, topRight, true)

  context:setColor(style.borderColor)
  context:drawRectOutline(globalRect)
end
