module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.event.EventManager")
require("lost.guiro.MouseManager")


--[[
     Screen class
  ]]
Screen = lost.common.Class("lost.guiro.Screen", lost.guiro.View)

function Screen:__init(properties)
  properties = properties or {}

  -- initialize defaults
  properties.focusable = true

  lost.guiro.View.__init(self, properties)

  log.debug("----------------------------------------------------------")
  self.eventManager = lost.guiro.event.EventManager(self)
  self.mouseManager = lost.guiro.MouseManager(self)

	self.parent = nil
end

function Screen:__finalize()
  log.debug("-------------- Screen finalize")
end

function Screen:propagateMouseEvent(event)
  self.eventManager:propagateMouseEvent(self, event)
end

function Screen:propagateKeyEvent(event)
  self.eventManager:propagateKeyEvent(event)
end

--[[
    only accepts lost.guiro.UserInterface
  ]]
function Screen:appendChild(child)
  if child:is("lost.guiro.UserInterface") then
    lost.guiro.View.appendChild(self, child)
		child:setParent(self)
  else
    log.error("Screen:appendChild() cannot append ".. child:className())
  end
end

--[[
    Screen is always dirty
  ]]
function Screen:render(canvas, forceRender)
  for k,ui in next,self.children do
    ui:render(canvas, forceRender)
  end
  -- always draw mouse on top if a custom one exists
  if self.mouseManager.currentMouse then
    self.mouseManager.currentMouse:render(canvas, forceRender)
  end
end

function Screen:setEventDispatcher(dispatcher)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:propagateMouseEvent(event) end)
  dispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:propagateKeyEvent(event) end)
  dispatcher:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self:propagateKeyEvent(event) end)
end

function Screen:screen()
  return self
end
