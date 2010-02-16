--[[
    lost.guiro.ReceivesEvents
    
    ReceivesEvents is an interface that receives all events sent through the application event subsystem.
    Sets up event handlers for low level events arriving from tasklet/application.
  ]]
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.event.EventManager")

lost.common.Class "lost.guiro.ReceivesEvents" {}

using "lost.guiro.event.EventManager"
using "lost.application.currentTasklet"

function ReceivesEvents:constructor()
  -- setup event manager
  self.eventManager = EventManager(self)

  if currentTasklet ~= nil then
    self:setEventDispatcher(currentTasklet.eventDispatcher)
  end
end

function ReceivesEvents:setEventDispatcher(eventDispatcher)
  if eventDispatcher ~= nil then
    eventDispatcher:addEventListener(lost.application.DropEvent.DROPPED_FILE, function(event) self:propagateDropEvent(event) end)
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
  end
end

function ReceivesEvents:propagateDropEvent(event)
  self.eventManager:propagateDropEvent(self, event)
end

function ReceivesEvents:propagateMouseEvent(event)
  self.eventManager:propagateMouseEvent(self, event)
end

function ReceivesEvents:propagateKeyEvent(event)
  self.eventManager:propagateKeyEvent(event)
end

function ReceivesEvents:propagateTouchEvent(event)
  self.eventManager:propagateTouchEvent(self, event)
end
