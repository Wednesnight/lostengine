module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.event.EventManager")
require("lost.guiro.TextureManager")
require("lost.common.ShaderFactory")

local _ui = nil
lost.guiro.ui = function()
  if not _ui then
    _ui = lost.guiro.view.UserInterface()
  end
  return _ui
end

lost.common.Class "lost.guiro.view.UserInterface" "lost.guiro.view.View" {}

using "lost.guiro.event.EventManager"

function UserInterface:constructor()
  lost.guiro.view.View.constructor(self)
  self.id = "ui"
  self.eventManager = EventManager(self)
  self:setEventDispatcher(tasklet.eventDispatcher)

  self:bounds(lost.guiro.Bounds(0, 0, tasklet.window.size.width, tasklet.window.size.height))

  self.focusable = true

  self.camera = lost.camera.Camera2D.create(lost.math.Rect())
  self.layer.renderNode:add(lost.rg.Camera.create(self.camera))
  self.layer.renderNode:add(lost.rg.DepthTest.create(false))
  
  tasklet.uiNode:add(self.layer.renderNode)  
end

function UserInterface:setEventDispatcher(eventDispatcher)
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

  eventDispatcher:addEventListener(lost.application.ResizeEvent.TASKLET_WINDOW_RESIZE, function(event) 
        self._bounds.width = lost.guiro.wsize{abs=event.width}
        self._bounds.height = lost.guiro.hsize{abs=event.height}
        self:needsLayout()
      end)
end

function UserInterface:propagateDragNDropEvent(event)
  self.eventManager:propagateDragNDropEvent(self, event)
end

function UserInterface:propagateMouseEvent(event)
  self.eventManager:propagateMouseEvent(self, event)
end

function UserInterface:propagateKeyEvent(event)
  self.eventManager:propagateKeyEvent(event)
end

function UserInterface:propagateTouchEvent(event)
  self.eventManager:propagateTouchEvent(self, event)
end

function UserInterface:updateLayout()
  lost.guiro.view.View.updateLayout(self)
  self.camera:viewport(self.rect)
end