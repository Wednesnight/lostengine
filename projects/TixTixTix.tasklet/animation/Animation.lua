module("lost.animation", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.animation.Animation" {}

function Animation:constructor()
  self.running = false
  self.listeners = {}
end

--[[
    xxxxxx  Public methods  xxxxxx
  ]]

function Animation:start()
  if self:dispatchEvent("animationStart") then
    self.running = true
    self.passedSec = lost.platform.currentTimeSeconds()
    lost.common.callLater(Animation.__process, self)
  end
end

function Animation:stop()
  if self:dispatchEvent("animationStop") then
    self.running = false
  end
end

function Animation:cancel()
  self:_finish()
end

--[[
    xxxxxx  Private methods  xxxxxx
  ]]

--[[
    Override this to add your update code, base method must be called after update is done
  ]]
function Animation:_update(deltaSec)
  return self:dispatchEvent("animationUpdate", deltaSec)
end

--[[
    Override this method to add your cleanup code
  ]]
function Animation:_finish()
  self.running = false
  self:dispatchEvent("animationFinished")
end

--[[
    Do not override, main animation loop
  ]]
function Animation:__process()
  local currentSec = lost.platform.currentTimeSeconds()
  local deltaSec = currentSec - self.passedSec
  if not self:_update(deltaSec) and self.running then
    lost.common.callLater(Animation.__process, self)
    self.passedSec = currentSec
  elseif self.running then
    self:_finish()
  end
end

--[[
    xxxxxx  Events  xxxxxx
  ]]

--[[ 
    add event listener, possible events:
      animationStart:    called before animation is started
      animationStop:     called before animation is stopped
      animationUpdate:   called on animation update
      animationFinished: called after animation is finished
  ]]
function Animation:addEventListener(which, listener)
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
function Animation:dispatchEvent(which, param)
  local result = true
  if self.listeners[which] then
    for k,listener in next,self.listeners[which] do
      local listenerResult = listener(self, param)
      if type(listenerResult) == "boolean" then
        result = result and listenerResult
      end
    end
  end
  return result
end

--[[
    removes the provided function from the listener list of the given event type
  ]]
function Animation:removeEventListener(which, listener)
  if self.listeners[which] then
    local t = self.listeners[which]
    for k,v in ipairs(t) do
      if v == listener then
        table.remove(t, k)
      end
    end
  end
end
