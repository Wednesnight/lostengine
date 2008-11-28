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
    self.textureParams.minFilter = image.filter
    self.textureParams.magFilter = image.filter
    self.texture = lost.gl.Texture()
    self.texture:init(image.bitmap, self.textureParams)
  end
  self.texture:bind()
  self.texture:filter(image.filter)

  context:setColor(lost.common.Color(image.color.r, image.color.g, image.color.b, image.alpha))
  local imageRect = lost.math.Rect(globalRect)
  if not image.stretch then
    local ratio = lost.math.Vec2(globalRect.width/image.frameSize.width, globalRect.height/image.frameSize.height)
    if (ratio.x <= ratio.y) then
      imageRect.width  = math.min(imageRect.width, image.frameSize.width * ratio.x)
      imageRect.height = math.min(imageRect.height, image.frameSize.height * ratio.x)
    else
      imageRect.width  = math.min(imageRect.width, image.frameSize.width * ratio.y)
      imageRect.height = math.min(imageRect.height, image.frameSize.height * ratio.y)
    end
  end

  if self.currentStep == nil then
    self.currentStep = 0
  end
  if self.lastTimestamp == nil or os.clock() - self.lastTimestamp >= image.interval then
    self.currentStep = self.currentStep + 1
    if self.currentStep == image.bitmap.width / image.frameSize.width then
      self.currentStep = 0
    end
    self.lastTimestamp = os.clock()
  end
  local tmpRect = lost.math.Rect(image.frameSize)
  tmpRect.x = tmpRect.width * self.currentStep
  local bottomLeft = lost.math.Vec2(tmpRect.x/image.bitmap.width, 0)
  local bottomRight = lost.math.Vec2((tmpRect.x + tmpRect.width)/image.bitmap.width, 0)
  local topLeft = lost.math.Vec2(tmpRect.x/image.bitmap.width, 1)
  local topRight = lost.math.Vec2((tmpRect.x + tmpRect.width)/image.bitmap.width, 1)
  context:drawRectTextured(imageRect, self.texture, bottomLeft, bottomRight, topLeft, topRight, true)

  context:setColor(style.borderColor)
  context:drawRectOutline(globalRect)

  -- FIXME: currently we have to update every frame  
  image:needsRedraw()
end
