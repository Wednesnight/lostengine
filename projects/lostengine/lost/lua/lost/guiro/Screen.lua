module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.event.EventManager")
--require("lost.guiro.MouseManager")


--[[
     Screen
     Holds only UserInterfaces.
     Sets up basic 2D camera and other flags for rendering.
     Sets up event handlers for low level events arriving from tasklet/application.
     
     The Screen is the only view who's global rect is controlled from the outside. 
     A Screen should always span the whole physical window area OR the area of the
     physical window that you want to have covered by the guiro user interfaces.
     Therefore, you need to attach a listener to the tasklets EventDispatcher and 
     listen for resize events. If the physical window resizes, you need to tell the
     Screen the new size, so it can track it and force an update of the subviews
     if required.
  ]]
lost.common.Class "lost.guiro.Screen" "lost.guiro.View" {}

function Screen:constructor()
  lost.guiro.View.constructor(self)
  self.focusable = true
  self.eventManager = lost.guiro.event.EventManager(self)
--  self.mouseManager = lost.guiro.MouseManager(self)

  -- disable scissoring when drawing is done
  local disableScissorNode = lost.rg.Scissor.create(false)
  disableScissorNode.name = "disableScissorNode"
  self.rootNode:add(disableScissorNode)

	self.parent = nil

--[[ FIXME: we need to move this stuff into the RenderGraph
  -- initialize scissor state - we need to make sure that scissoring is disabled when done
  self.scissorState = lost.gl.State.create(lost.gl.Scissor.create(false))
]]

  self.camera = lost.camera.Camera2D.create(lost.math.Rect())
  self.renderNode:add(lost.rg.ClearColor.create(lost.common.Color(0,0,0,1)))
  self.renderNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  self.renderNode:add(lost.rg.Camera.create(self.camera))
  self.renderNode:add(lost.rg.DepthTest.create(false))
  self:showBackground(false)
  self:showFrame(false)
end

function Screen:propagateMouseEvent(event)
  self.eventManager:propagateMouseEvent(self, event)
end

function Screen:propagateKeyEvent(event)
  self.eventManager:propagateKeyEvent(event)
end

function Screen:propagateTouchEvent(event)
  self.eventManager:propagateTouchEvent(self, event)
end

--[[
    only accepts lost.guiro.UserInterface
    will fail if subview is not a lost.common.Class
  ]]
function Screen:addSubview(subview)
  if subview:isDerivedFrom("lost.guiro.UserInterface") then
    lost.guiro.View.addSubview(self, subview)
		subview:setParent(self)
  else
    error("Screen:addSubview() can only add subviews of type UserInterface : ".. subview:className(), 2)
  end
end

function Screen:listenTo(dispatcher)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_SCROLL, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:propagateKeyEvent(event) end)
  dispatcher:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self:propagateKeyEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.ResizeEvent.MAIN_WINDOW_RESIZE, function(event) 
        local resizeEvent = lost.application.ResizeEvent.cast(event)
        self.bounds.width = lost.guiro.wabs(resizeEvent.width)
        self.bounds.height = lost.guiro.habs(resizeEvent.height)
        callLater(function() self:updateLayout(true) end)
      end)
end

--[[
    screen is overridden and called by all other subviews on parent
  ]]
function Screen:screen()
  return self
end

-- every time the screens layout is updated we MUST reset the viewport 
-- so we first call update on the base class, then use the resulting 
function Screen:updateLayout(forceUpdate)
  local doUpdateLayout = forceUpdate or self.dirtyLayout
  lost.guiro.View.updateLayout(self, forceUpdate)
  if doUpdateLayout then
    self.camera:viewport(self.currentGlobalRect)
  end
end
