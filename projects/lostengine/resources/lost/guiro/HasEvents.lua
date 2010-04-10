--[[
    lost.guiro.HasEvents
    
    HasEvents is an interface that implements all event related functionality of a Guiro View.
  ]]
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.event.Event")
require("lost.guiro.event.EventDispatcher")

lost.common.Class "lost.guiro.HasEvents" {}

using "lost.guiro.event.Event"
using "lost.guiro.event.EventDispatcher"
using "lost.application.currentTasklet"

function HasEvents:constructor()
  -- setup event dispatchers
  self.captureEventDispatcher = EventDispatcher()
  self.targetEventDispatcher = EventDispatcher()
  self.bubbleEventDispatcher = EventDispatcher()
end

--[[ 
    Basically same functionality as EventDispatcher:addEventListener.
    If phase is set, the listener is only added to associated event listeners.
    If phase is not set, the listener is only added to the target listeners
  ]]
function HasEvents:addEventListener(which, listener, phase)
--  log.debug(tostring(self.id).. ": adding listener for ".. phase)

  -- default is target + bubble
  local capture = (phase ~= nil and phase == Event.PHASE_CAPTURE)
  local target = (phase == nil or phase == Event.PHASE_TARGET)
  local bubble = (phase == nil or phase == Event.PHASE_BUBBLE)

  if capture then 
    self.captureEventDispatcher:addEventListener(which, listener)
  end
  if target then 
    self.targetEventDispatcher:addEventListener(which, listener)
  end
  if bubble then 
    self.bubbleEventDispatcher:addEventListener(which, listener)
  end
end

--[[ 
    Basically same functionality as EventDispatcher:removeEventListener.
    If phase is set, the listener is only removed from the associated event listeners.
    If phase is not set, the listener is only removed from the target listeners
  ]]
function HasEvents:removeEventListener(which, listener, phase)
--  log.debug(tostring(self.id).. ": removing listener for ".. phase)

  -- default is target + bubble
  local capture = (phase ~= nil and phase == Event.PHASE_CAPTURE)
  local target = (phase == nil or phase == Event.PHASE_TARGET)
  local bubble = (phase == nil or phase == Event.PHASE_BUBBLE)

  if capture then 
    self.captureEventDispatcher:removeEventListener(which, listener)
  end
  if target then 
    self.targetEventDispatcher:removeEventListener(which, listener)
  end
  if bubble then 
    self.bubbleEventDispatcher:removeEventListener(which, listener)
  end
end

--[[
    Calling dispatchEvent on a view notifies all target listeners and bubbles the event
    if event.bubbles is enabled.
    
    If phase is set, dispatchEvent will only notify the appropriate listeners
  ]]
function HasEvents:dispatchEvent(event, phase)
  -- accept valid events only
  if event ~= nil then
    -- default is target + bubble
    local capture = (phase ~= nil and phase == Event.PHASE_CAPTURE)
    local target = (phase == nil or phase == Event.PHASE_TARGET)
    local bubble = (phase == nil or phase == Event.PHASE_BUBBLE)

    if capture then
--      log.debug("dispatch capture")
      -- target == self since self dispatches the event
      event.target = self
      -- build capture hierarchy
      local stack = {}
      local current = self.parent
      while current ~= nil do
        table.insert(stack, 1, current)
        current = current.parent
      end
      -- dispatch capture
      for index, view in next,stack do
        event.currentTarget = view
        self.captureEventDispatcher:dispatchEvent(event)
        if event.stopDispatch then
          break
        end
      end
    end

    if target and not event.stopDispatch then
--      log.debug("dispatch target")
      event.currentTarget = self
      self.targetEventDispatcher:dispatchEvent(event)
    end

    if event.bubbles and not event.stopPropagation then
--      log.debug("dispatch bubble")
      local currentTarget = self.parent
      while currentTarget ~= nil and not event.stopPropagation do
        event.currentTarget = currentTarget
        self.bubbleEventDispatcher:dispatchEvent(event)
        currentTarget = currentTarget.parent
      end
    end
  end
end

--[[
    Routes the event to the appropriate EventDispatcher
  ]]
function HasEvents:routeEvent(event)
  if event.phase == Event.PHASE_CAPTURE then
    self.captureEventDispatcher:dispatchEvent(event)
  elseif event.phase == Event.PHASE_TARGET then
    self.targetEventDispatcher:dispatchEvent(event)
  elseif event.phase == Event.PHASE_BUBBLE then
    self.bubbleEventDispatcher:dispatchEvent(event)
  end
end
