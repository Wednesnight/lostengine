module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.event.EventManager")
require("lost.guiro.TextureManager")
require("lost.common.ShaderFactory")
require("lost.common.MeshFactory")

lost.common.Class "lost.guiro.view.UserInterface" "lost.guiro.view.View" {}

using "lost.guiro.event.EventManager"

function UserInterface:constructor(textureManager)
  lost.guiro.view.View.constructor(self, textureManager)

  self.textureManager = lost.guiro.TextureManager(256)
  self.shaderFactory = lost.common.ShaderFactory(tasklet.loader)
  self.meshFactory = lost.common.MeshFactory(self.shaderFactory)
  self.eventManager = EventManager(self)
  self:setEventDispatcher(tasklet.eventDispatcher)

  -- init size
  local windowRect = tasklet.window.params.rect
  self.bounds = lost.guiro.Bounds(0, 0, windowRect.width, windowRect.height)

  self.focusable = true
	self.parent = nil

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

  eventDispatcher:addEventListener(lost.application.ResizeEvent.MAIN_WINDOW_RESIZE, function(event) 
        self.bounds.width = lost.guiro.wsize{abs=event.width}
        self.bounds.height = lost.guiro.hsize{abs=event.height}
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

function UserInterface:afterLayout()
  self.camera:viewport(self.rect)
end

function UserInterface:layerNeedsUpdate(layer)
  log.debug("layer would need update: "..layer.id)
end

function UserInterface:layerNeedsLayout(layer)
  log.debug("layer would need layout: "..layer.id)
end

function UserInterface:layerNeedsDisplay(layer)
  log.debug("layer would need display: "..layer.id)
end
