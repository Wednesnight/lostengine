module("lost.guiro.event", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.event.EventDispatcher" {}

function EventDispatcher:constructor()
  self.listeners = {}
end

--[[ 
    Lua implementation of the EventDispatcher with no connection to the C++ implementation
  ]]
function EventDispatcher:addEventListener(which, listener)
  if type(which) == "string" then
    which = lost.common.djb2Hash(which)
  end
  local l = self.listeners[which]
  if not l then
    l = {}
  end
  table.insert(l, listener)
  self.listeners[which] = l
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
  if type(which) == "string" then
    which = lost.common.djb2Hash(which)
  end
  if self.listeners[which] then
    local t = self.listeners[which]
    for k,v in ipairs(t) do
      if v == listener then
        table.remove(t, k)
      end
    end
  end
end
