guiro:Renderer
{
  render = function(self, context, image, style)
    local globalRect = image:globalRect()
    local imageRect = lost.math.Rect(globalRect)
    
    if not self.texture then
      self.textureParams = lost.gl.Texture.Params()
      self.textureParams.minFilter = gl.GL_LINEAR
      self.textureParams.magFilter = gl.GL_LINEAR
      self.texture = lost.gl.Texture()
      self.texture:init(image.bitmap, self.textureParams)
    end
    if not image.stretch then
      local ratio = lost.math.Vec2(image.bounds.width/image.bitmap.width, image.bounds.height/image.bitmap.height)
      if (ratio.x <= ratio.y) then
        imageRect.width  = math.min(imageRect.width, image.bitmap.width * ratio.x)
        imageRect.height = math.min(imageRect.height, image.bitmap.height * ratio.x)
      else
        imageRect.width  = math.min(imageRect.width, image.bitmap.width * ratio.y)
        imageRect.height = math.min(imageRect.height, image.bitmap.height * ratio.y)
      end
    end

    context:setColor(lost.common.Color(1,1,1,image.alpha))
    context:drawRectTextured(imageRect, self.texture, true)
    context:setColor(lost.common.Color(0,0,0))
    context:drawRectOutline(globalRect)
  end
}
