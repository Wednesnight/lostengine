module("lost.guiro", package.seeall)

--[[
     UserInterface class
  ]]
require("lost.guiro.View")

class "lost.guiro.UserInterface" (lost.guiro.View)
UserInterface = _G["lost.guiro.UserInterface"]

lost.guiro.View:addBase(UserInterface, "UserInterface")

function UserInterface:__init() super()
end

--[[
    only accepts lost.guiro.controls.Window
  ]]
function UserInterface:appendChild(child)
  if child:is("Window") then
    lost.guiro.View.appendChild(self, child)
  else
    log.error("UserInterface:appendChild() cannot append ".. child:className())
  end
end

--[[
    initializes renderbuffer, texures, states
  ]]
function UserInterface:initialize(context)
  if not self.initialized then
    self.initialized = true

    -- helper vars
    local globalRect = self:globalRect()

    -- initialize render buffer
    self.renderBuffer = lost.gl.FrameBuffer()
    self.renderBuffer:enable()

    self.depthTexture = lost.gl.Texture()
    self.depthTexture:bind()
    self.depthTexture:init(0, gl.GL_DEPTH_COMPONENT32, globalRect.width, globalRect.height, 0, gl.GL_DEPTH_COMPONENT, gl.GL_FLOAT)
    self.renderBuffer:attachDepth(self.depthTexture)

    self.colorTexture = lost.gl.Texture()
    self.colorTexture:bind()
    self.colorTexture:init(0, gl.GL_RGBA8, globalRect.width, globalRect.height, 0, gl.GL_RGBA, gl.GL_UNSIGNED_BYTE)
    self.renderBuffer:attachColor(0, self.colorTexture)
    self.colorTexture:filter(gl.GL_NEAREST)
    self.colorTexture:wrap(gl.GL_CLAMP)

    if (self.renderBuffer:status() ~= gl.GL_FRAMEBUFFER_COMPLETE_EXT) then
      --log.error("FrameBuffer status: ".. lost.gl.enum2string(self.renderBuffer:status()))
    end
    self.renderBuffer:disable()

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
end

--[[
    activates renderbuffer and checks children for redraw flag
  ]]
function UserInterface:render(context, forceRender)
  -- initialize renderbuffer
  self:initialize(context)

  -- helper vars
  local globalRect = self:globalRect()

  -- update renderbuffer
  self.renderBuffer:enable()
  context:pushState(self.bufferState)

  gl.glPushAttrib(gl.GL_VIEWPORT_BIT)
  gl.glViewport(0, 0, globalRect.width, globalRect.height)
  context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(globalRect.width, globalRect.height))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()

  if forceRender or self.dirty then
    context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
    lost.guiro.View.render(self, context, forceRender)
  else
    self:renderChildren(self, context)
  end

  gl.glPopAttrib()
  context:popState()
  self.renderBuffer:disable()

  -- draw renderbuffer
  context:pushState(self.renderState)
  context:setColor(lost.common.Color(1,1,1))
  context:drawRectTextured(lost.math.Rect(globalRect.x, globalRect.y, globalRect.width + 1, globalRect.height + 1), self.colorTexture, false)
  context:popState()
end

--[[
    checks children for redraw flag
  ]]
function UserInterface:renderChildren(parent, context)
  for k,child in next,parent.children do
    if child.dirty then
      child:render(context)
    else
      self:renderChildren(child, context)
    end
  end
end
