module("lost.guiro", package.seeall)

require("lost.guiro.View")

--[[
     UserInterface class
  ]]
UserInterface = lost.common.Class("lost.guiro.UserInterface", lost.guiro.View)

function UserInterface:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  self.renderState = lost.gl.State.create(lost.gl.DepthTest.create(false),
                                          lost.gl.Blend.create(false),
                                          lost.gl.Texture2D.create(false),
                                          lost.gl.NormalArray.create(false),
                                          lost.gl.VertexArray.create(false),
                                          lost.gl.TextureArray.create(false))

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
    render
  ]]
function UserInterface:render(canvas, forceRender)
  -- helper vars
  local globalRect = self:globalRect()

  canvas.context:pushState(self.renderState)
  if forceRender or self.dirty then
    canvas:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
    lost.guiro.View.render(self, canvas, true)
  else
    self:renderChildren(self, canvas)
  end
  canvas.context:popState()
end

--[[
    checks children for redraw flag
  ]]
function UserInterface:renderChildren(parent, canvas)
  local topWindow = self:topWindow()
  for k,child in next,parent.children do
    child:update(canvas)
    if child.dirty then
      child:render(canvas)
      if topWindow then
        topWindow:render(canvas, true)
      end
    else
      self:renderChildren(child, canvas)
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
