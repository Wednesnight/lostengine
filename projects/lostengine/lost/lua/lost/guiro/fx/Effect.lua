module("lost.guiro.fx", package.seeall)

--[[
     Effect
  ]]
class "lost.guiro.fx.Effect"
Effect = _G["lost.guiro.fx.Effect"]

require("lost.guiro.View")

function Effect:__init(view, params)
  self.view = view
  self.params = params

  if (self.view) then
    -- patch render calls
    self.renderView = self.view.render
    self.view.render = function(view, context) self:render(context) end
    
    -- initialize render buffer
    self.renderBuffer = lost.gl.FrameBuffer()
    self.renderBuffer:enable()

    self.depthTexture = lost.gl.Texture()
    self.depthTexture:bind()
    self.depthTexture:init(0, gl.GL_DEPTH_COMPONENT32, self.view.bounds.width, self.view.bounds.height, 0, gl.GL_DEPTH_COMPONENT, gl.GL_FLOAT)
    self.renderBuffer:attachDepth(self.depthTexture)

    self.colorTexture = lost.gl.Texture()
    self.colorTexture:bind()
    self.colorTexture:init(0, gl.GL_RGBA8, self.view.bounds.width, self.view.bounds.height, 0, gl.GL_RGBA, gl.GL_UNSIGNED_BYTE)
    self.renderBuffer:attachColor(0, self.colorTexture)
    self.colorTexture:filter(gl.GL_NEAREST)
    self.colorTexture:wrap(gl.GL_CLAMP)

    if (self.renderBuffer:status() ~= gl.GL_FRAMEBUFFER_COMPLETE_EXT) then
--      log.error("FrameBuffer status: ".. lost.gl.enum2string(self.renderBuffer:status()))
    end
    self.renderBuffer:disable()
  end
end

function Effect:render(context)
  if not self.renderState then
    self.renderState = context:copyState()
    self.renderState.alphaTest = true
    self.renderState.clearColor = lost.common.Color(0,0,0)
    self.renderState.depthTest = false
    self.renderState.blend = true
    self.renderState.blendSrc = gl.GL_SRC_ALPHA
    self.renderState.blendDest = gl.GL_ONE_MINUS_SRC_ALPHA
    self.renderState.texture2D = true
    self.renderState.normalArray = false
    self.renderState.vertexArray = true
    self.renderState.textureCoordArray = true
  end
  if not self.bufferState then
    self.bufferState = context:copyState()
    self.bufferState.alphaTest = true
    self.bufferState.clearColor = lost.common.Color(0,0,0)
    self.bufferState.depthTest = false
    self.bufferState.blend = false
    self.bufferState.texture2D = false
    self.bufferState.normalArray = false
    self.bufferState.vertexArray = true
    self.bufferState.textureCoordArray = false
  end

  self.renderBuffer:enable()
  context:pushState(self.bufferState)
  context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)

  gl.glPushAttrib(gl.GL_VIEWPORT_BIT)
  gl.glViewport(0, 0, self.view.bounds.width, self.view.bounds.height)
  context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(self.view.bounds.width, self.view.bounds.height))

  self.renderView(self.view, context)

  gl.glPopAttrib()
  context:popState()
  self.renderBuffer:disable()

  self:renderEffect(context)
end

function Effect:renderEffect(context)
  context:pushState(self.renderState)

  context:setColor(lost.common.Color(1,1,1))
  local rect = lost.math.Rect(self.view.bounds)
  context:drawRectTextured(rect, self.colorTexture, false)

  context:popState()
end

function Effect:finish()
  self.view.render = self.renderView
end
