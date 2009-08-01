module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.event.EventManager")
require("lost.guiro.MouseManager")


--[[
     Screen class
  ]]
lost.common.Class "lost.guiro.Screen" "lost.guiro.View" {}

function Screen:create(properties)
  self.focusable = true
  lost.guiro.View.create(self, properties)

  log.debug("----------------------------------------------------------")
  self.eventManager = lost.guiro.event.EventManager(self)
  self.mouseManager = lost.guiro.MouseManager(self)

	self.parent = nil

--[[ FIXME: we need to move this stuff into the RenderGraph
  -- initialize scissor state - we need to make sure that scissoring is disabled when done
  self.scissorState = lost.gl.State.create(lost.gl.Scissor.create(false))
]]
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
  ]]
function Screen:addSubview(child)
  if child:is("lost.guiro.UserInterface") then
    lost.guiro.View.addSubview(self, child)
		child:setParent(self)
  else
    log.error("Screen:addSubview() can only add subviews of type UserInterface".. child:className())
  end
end

function Screen:setEventDispatcher(dispatcher)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:propagateKeyEvent(event) end)
  dispatcher:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self:propagateKeyEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, function(event) self:propagateTouchEvent(event) end)
  dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:propagateTouchEvent(event) end)
end

function Screen:screen()
  return self
end

function Screen:initialize()
  log.debug("Screen:initialize()")

  local globalRect = self:globalRect()

  self.camera = lost.camera.Camera2D.create(lost.math.Rect(globalRect.x, globalRect.y, globalRect.width, globalRect.height))

  self.renderNode:add(lost.rg.ClearColor.create(lost.common.Color(0,0,0,1)))
  self.renderNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  self.renderNode:add(lost.rg.Camera.create(self.camera))
  self.renderNode:add(lost.rg.DepthTest.create(false))
  
  lost.guiro.View.initialize(self)
end
