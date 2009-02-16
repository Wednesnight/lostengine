module("lost.guiro.event", package.seeall)

class "lost.guiro.event.EventDispatcher"
EventDispatcher = _G["lost.guiro.event.EventDispatcher"]

function EventDispatcher:__init() 
  self.listeners = {}
end

--[[ 
    Lua implementation of the EventDispatcher with no connection to the C++ implementation
  ]]
function EventDispatcher:addEventListener(which, listener)
  local l = self.listeners[which]
  if not l then
    l = {}
  end
  table.insert(l, listener)
  self.listeners[which] = l
--  log.debug("added listener "..tostring(listener).."for "..which.." "..tostring(#l).." "..tostring(l))
end

--[[ 
    calls all listener functions for the specific event type
  ]]
function EventDispatcher:dispatchEvent(event)
--  log.debug("dispatching event: "..event.type)
  if self.listeners[event.type] then
--    log.debug("found "..tostring(#self.listeners[event.type]).." listeners")
    for k,listener in next,self.listeners[event.type] do
      listener(event)
--      log.debug("calling listeners for event:"..event.type)   
    end
  else
--    log.debug("found no listeners")
  end
end

-- removes the provided function from the listener list of the given event type
function EventDispatcher:removeEventListener(which, listener)
  if self.listeners[which] then
    local t = self.listeners[which]
    for k,v in t do
      if v == listener then
        table.remove(t, k)
        log.debug("removed listener for "..which)
      end
    end
  end
end
