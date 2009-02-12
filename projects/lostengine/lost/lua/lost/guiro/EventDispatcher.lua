module("lost.guiro", package.seeall)

class "lost.guiro.EventDispatcher"
EventDispatcher = _G["lost.guiro.EventDispatcher"]

function EventDispatcher:__init() 
  self.listeners = {}
end

--[[ 
    Lua implementation of the EventDispatcher with no connection to the C++ implementation
  ]]
function EventDispatcher:addEventListener(which, listener)
  if not self.listeners[which] then
    self.listeners[which] = {}
  end
  table.insert(self.listeners[which], listener)
end

--[[ 
    calls all listener functions for the specific event type
  ]]
function EventDispatcher:dispatchEvent(event)
  if self.listeners[event.type] then
    for k,listener in next,self.listeners[event.type] do
      listener(event)
    end
  end
end

-- removes the provided function from the listener list of the given event type
function EventDispatcher:removeEventListener(which, listener)
  if self.listeners[which] then
    local t = self.listeners[which]
    for k,v in t do
      if v == listener then
        table.remove(t, k)
      end
    end
  end
end
