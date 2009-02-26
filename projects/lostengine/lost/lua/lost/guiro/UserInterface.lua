module("lost.guiro", package.seeall)

require("lost.guiro.View")

--[[
     UserInterface class
  ]]
UserInterface = lost.common.Class("lost.guiro.UserInterface", lost.guiro.View)

function UserInterface:__init() lost.guiro.View.__init(self)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:updateFocus(event) end)
end

--[[
    only accepts lost.guiro.controls.Window
  ]]
function UserInterface:appendChild(child)
  if child:is("lost.guiro.controls.Window") then
    lost.guiro.View.appendChild(self, child)
  else
    log.error("UserInterface:appendChild() cannot append ".. child:className())
  end
end

--[[
    initialization
  ]]
function UserInterface:initialize(context)
  if not self.initialized then
    self.initialized = true

    -- helper vars
    local globalRect = self:globalRect()

    self.renderState = context:copyState()
    self.renderState.depthTest = false
    self.renderState.blend = false
    self.renderState.texture2D = false
    self.renderState.normalArray = false
    self.renderState.vertexArray = true
    self.renderState.textureCoordArray = false
  end
end

--[[
    render
  ]]
function UserInterface:render(context, forceRender)
  self:initialize(context)

  -- helper vars
  local globalRect = self:globalRect()

  context:pushState(self.renderState)
  if forceRender or self.dirty then
    context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
    lost.guiro.View.render(self, context, true)
  else
    gl.glEnable(gl.GL_SCISSOR_TEST)
    self:renderChildren(self, context)
    gl.glDisable(gl.GL_SCISSOR_TEST)
  end
  context:popState()
end

--[[
    checks children for redraw flag
  ]]
function UserInterface:renderChildren(parent, context)
  local topWindow = self:topWindow()
  for k,child in next,parent.children do
    child:update(context)
    if child.dirty then
      local childRect = child:globalRect()
      gl.glScissor(childRect.x, childRect.y, childRect.width, childRect.height)
      child:render(context)
      if topWindow then
        topWindow:render(context, true)
      end
    else
      self:renderChildren(child, context)
    end
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
  local mouseEvent = event
  local topWindow = self:topWindow()
  if not topWindow or not topWindow:containsCoord(mouseEvent.pos) then
    local idx = table.maxn(self.children)
    while idx > 0 do
      local child = self.children[idx]
      if child:containsCoord(mouseEvent.pos) then
        table.remove(self.children, idx)
        table.insert(self.children, child)
        child:needsRedraw()
        break
      end
      idx = idx-1
    end
  end
end
