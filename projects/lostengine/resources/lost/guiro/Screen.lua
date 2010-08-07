module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.event.EventManager")

--[[
     Screen
     Holds only UserInterfaces.
     Sets up basic 2D camera and other flags for rendering.
     
     The Screen is the only view who's global rect is controlled from the outside. 
     A Screen should always span the whole physical window area OR the area of the
     physical window that you want to have covered by the guiro user interfaces.
     Therefore, you need to attach a listener to the tasklets EventDispatcher and 
     listen for resize events. If the physical window resizes, you need to tell the
     Screen the new size, so it can track it and force an update of the subviews
     if required.
  ]]
lost.common.Class "lost.guiro.Screen" "lost.guiro.View" {}

using "lost.guiro.event.EventManager"

function Screen:constructor(textureManager)
  lost.guiro.View.constructor(self, textureManager)

  self.eventManager = EventManager(self)
  self:setEventDispatcher(tasklet.eventDispatcher)

  -- init size
  local windowRect = tasklet.window.params.rect
  self.bounds = lost.guiro.Bounds(0, 0, windowRect.width, windowRect.height)

  self.focusable = true
	self.parent = nil

  -- disable scissoring when drawing is done
  local disableScissorNode = lost.rg.Scissor.create(false)
  disableScissorNode.name = "disableScissorNode"
  self.rootNode:add(disableScissorNode)

  self.camera = lost.camera.Camera2D.create(lost.math.Rect())
  self.renderNode:add(lost.rg.Camera.create(self.camera))
  self.renderNode:add(lost.rg.DepthTest.create(false))
  self:showBackground(false)
  self:showFrame(false)
  
  tasklet.uiNode:add(self.rootNode)

  -- trigger updates
  self:needsLayout()
  self:needsRedraw()
end

function Screen:setEventDispatcher(eventDispatcher)
  eventDispatcher:addEventListener(lost.application.DragNDropEvent.DRAG_ENTER, function(event) self:propagateDragNDropEvent(event) end)
  eventDispatcher:addEventListener(lost.application.DragNDropEvent.DRAG_UPDATE, function(event) self:propagateDragNDropEvent(event) end)
  eventDispatcher:addEventListener(lost.application.DragNDropEvent.DRAG_LEAVE, function(event) self:propagateDragNDropEvent(event) end)
  eventDispatcher:addEventListener(lost.application.DragNDropEvent.DROP, function(event) self:propagateDragNDropEvent(event) end)
  eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:propagateMouseEvent(event) end)
  eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:propagateMouseEvent(event) end)
  eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:propagateMouseEvent(event) end)
  eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_SCROLL, function(event) self:propagateMouseEvent(event) end)
  eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:propagateKeyEvent(event) end)
  eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self:propagateKeyEvent(event) end)
  eventDispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self:propagateTouchEvent(event) end)
  eventDispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self:propagateTouchEvent(event) end)
  eventDispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, function(event) self:propagateTouchEvent(event) end)
  eventDispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:propagateTouchEvent(event) end)

  eventDispatcher:addEventListener(lost.application.ResizeEvent.MAIN_WINDOW_RESIZE, function(event) 
        self.bounds.width = lost.guiro.wsize{abs=event.width}
        self.bounds.height = lost.guiro.hsize{abs=event.height}
        self:needsLayout()
      end)
end

function Screen:propagateDragNDropEvent(event)
  self.eventManager:propagateDragNDropEvent(self, event)
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
    every time the screens layout is updated we MUST reset the viewport 
  ]]
function Screen:afterLayout()
  self.camera:viewport(self.rect)
end

--[[
    only accepts lost.guiro.UserInterface
    will fail if subview is not a lost.common.Class
  ]]
function Screen:addSubview(subview)
  if subview:isDerivedFrom("lost.guiro.UserInterface") then
    lost.guiro.View.addSubview(self, subview)
  else
    error("Screen:addSubview() can only add subviews of type UserInterface : ".. subview:className(), 2)
  end
end
