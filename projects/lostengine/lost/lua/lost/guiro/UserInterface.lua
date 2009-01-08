module("lost.guiro", package.seeall)

--[[
     UserInterface class
  ]]
require("lost.guiro.View")

class "lost.guiro.UserInterface" (lost.guiro.View)
UserInterface = _G["lost.guiro.UserInterface"]

lost.guiro.View:addBase(UserInterface, "UserInterface")

function UserInterface:__init() lost.guiro.View.__init(self)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:updateFocus(event) end)
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

    self.colorTextureParams = lost.gl.Texture.Params()
    self.colorTexture = lost.gl.Texture()
    self.colorTexture:bind()
    self.colorTexture:init(lost.math.Vec2(globalRect.width, globalRect.height), self.colorTextureParams)
    self.renderBuffer:attachColor(0, self.colorTexture)
    self.colorTexture:filter(gl.GL_NEAREST)
    self.colorTexture:wrap(gl.GL_CLAMP_TO_EDGE)

    self.depthBuffer = lost.gl.RenderBuffer()
    self.depthBuffer:enable()
    self.depthBuffer:storage(lgl.LGL_DEPTH_COMPONENT16, self.colorTexture.width, self.colorTexture.height)
    self.renderBuffer:attachDepth(self.depthBuffer)
    self.depthBuffer:disable()

    if (self.renderBuffer:status() ~= lgl.LGL_FRAMEBUFFER_COMPLETE) then
      log.error("could not initialize FrameBuffer")
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

  -- fix viewport to fit texture size
  context:pushViewport(lost.math.Rect(0, 0, self.colorTexture.width, self.colorTexture.height))

  -- update renderbuffer
  self.renderBuffer:enable()
  context:pushState(self.bufferState)

  context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(self.colorTexture.width, self.colorTexture.height))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()
  if forceRender or self.dirty then
    context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
    lost.guiro.View.render(self, context, true)
  else
    self:renderChildren(self, context)
  end
  local topWindow = self:topWindow()
  if topWindow then
    topWindow:render(context, true)
  end

  context:popState()
  self.renderBuffer:disable()

  -- draw renderbuffer
  context:pushState(self.renderState)
  local displayAttributes = context:getDisplayAttributes()
  context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(displayAttributes.width, displayAttributes.height))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()
  context:setColor(lost.common.Color(1,1,1))
  context:drawRectTextured(lost.math.Rect(globalRect.x, globalRect.y, globalRect.width + 1, globalRect.height + 1), self.colorTexture, false)
  context:popState()

  -- restore viewport
  context:popViewport()
end

--[[
    checks children for redraw flag
  ]]
function UserInterface:renderChildren(parent, context)
  for k,child in next,parent.children do
    child:update(context)
    if child.dirty then
      child:render(context)
    else
      self:renderChildren(child, context)
    end
  end
end

--[[
    resize render buffer, textures
  ]]
function UserInterface:updateLayout(forceUpdate)
  if forceUpdate or self.dirtyLayout then
    local globalRect, localRect = lost.guiro.View.updateLayout(self, forceUpdate)
    if self.renderBuffer then
      self.renderBuffer:enable()

      self.colorTexture = lost.gl.Texture()
      self.colorTexture:bind()
      self.colorTexture:init(lost.math.Vec2(globalRect.width, globalRect.height), self.colorTextureParams)
      self.renderBuffer:attachColor(0, self.colorTexture)
      self.colorTexture:filter(gl.GL_NEAREST)
      self.colorTexture:wrap(gl.GL_CLAMP_TO_EDGE)

      self.depthBuffer = lost.gl.RenderBuffer()
      self.depthBuffer:enable()
      self.depthBuffer:storage(lgl.LGL_DEPTH_COMPONENT16, self.colorTexture.width, self.colorTexture.height)
      self.renderBuffer:attachDepth(self.depthBuffer)
      self.depthBuffer:disable()

      if (self.renderBuffer:status() ~= lgl.LGL_FRAMEBUFFER_COMPLETE) then
        log.error("could not initialize FrameBuffer")
      end
      self.renderBuffer:disable()
    end
    return globalRect, localRect
  else
    return lost.guiro.View.updateLayout(self, forceUpdate)
  end
end

--[[
    get focused window
  ]]
function UserInterface:topWindow()
  return self.children[table.maxn(self.children)]
end

--[[
    set focused window
  ]]
function UserInterface:updateFocus(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)
  local topWindow = self:topWindow()
  if not topWindow or not topWindow:containsCoord(mouseEvent.pos) then
    local idx = table.maxn(self.children)
    while idx > 0 do
      local child = self.children[idx]
      if child:containsCoord(mouseEvent.pos) then
        table.remove(self.children, idx)
        table.insert(self.children, child)
        break
      end
      idx = idx-1
    end
  end
end
