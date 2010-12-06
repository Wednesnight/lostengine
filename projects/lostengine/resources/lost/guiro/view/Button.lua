module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.view.Button" "lost.guiro.view.View" {}

using "lost.guiro.event.Event"

Button.STATE_NORMAL = "normal"  
Button.STATE_HOVER = "hover" 
Button.STATE_PUSHED = "pushed"  
Button.STATE_DISABLED = "disabled"

function Button:constructor(args)
	lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "button"
	self._state = Button.STATE_NORMAL
  self._allStates = {Button.STATE_NORMAL, Button.STATE_HOVER, Button.STATE_PUSHED, Button.STATE_DISABLED}
	self._enabled = true
  self._pushed = false;

  self._titles = {} -- one string per state

  self._handlerMaps = {}
  self._handlerMaps["normal"] = self:createNormalHandlerMap()
  self._handlerMaps["sticky"] = self:createStickyHandlerMap()
  self._handlerMaps["toggle"] = self:createToggleHandlerMap()
  self._currentHandlerMap = nil
	self:mode("normal")
	
  self:addEventListener("mouseEnter", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseLeave", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseDown", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseUpInside", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseUpOutside", function(event) self:eventHandler(event) end)	
end

function Button:pushed(val)
  if val ~= nil then
    if val then 
      self._pushed = true
    else
      self._pushed = false
    end
    if self._pushed then
      self:state(Button.STATE_PUSHED)
    else
      self:state(Button.STATE_NORMAL)
    end
  else
    return self._pushed
  end
end

function Button:mode(modeName)
  self._currentHandlerMap = self._handlerMaps[modeName]
  if self._currentHandlerMap == nil then
    log.warn("couldn't find handlerMap for mode: '"..tostring(modeName).."', defaulting to normal")
    self._currentHandlerMap = self._handlerMaps["normal"]
  end
end

function Button:eventHandler(event)
  if self._enabled then
    local handler = self._currentHandlerMap[event.type]
    if handler ~= nil then
      handler(event)
    end
  end
end

-- doesn't affect pushed value because state change is only momentary and not really necessary for handlers
-- FIXME: is this correct?
function Button:createNormalHandlerMap()
  return {
    mouseEnter = function(event) 
                  self:state(Button.STATE_HOVER) 
                  end,
    mouseLeave = function(event)
                  self:state(Button.STATE_NORMAL) 
                  end,
    mouseDown = function(event)
                  self:state(Button.STATE_PUSHED)
                  self:dispatchButtonEvent("buttonDown")    
                  end,
    mouseUpInside = function(event)
                      self:state(Button.STATE_HOVER)
                      self:dispatchButtonEvent("buttonClick")    
                      self:dispatchButtonEvent("buttonUp")    
                      end,
    mouseUpOutside = function(event)
                       self:state(Button.STATE_NORMAL)
                       self:dispatchButtonEvent("buttonUp")    
                       end,
  }
end

function Button:createStickyHandlerMap()
  return {
    mouseEnter = function(event) 
                    if not self:pushed() then
                      self:state(Button.STATE_HOVER) 
                    end
                  end,
    mouseLeave = function(event)
                    if not self:pushed() then
                      self:state(Button.STATE_NORMAL) 
                    end
                  end,
    mouseDown = function(event)
                    if not self:pushed() then
                      self:state(Button.STATE_PUSHED)
                      self:dispatchButtonEvent("buttonDown")    
                    end
                  end,
    mouseUpInside = function(event)
                        if not self:pushed() then      
                          self:pushed(true)
                          self:dispatchButtonEvent("buttonClick")
                        end
                      end,
    mouseUpOutside = function(event)
                         if not self:pushed() then      
                           self:state(Button.STATE_NORMAL)
                           self:dispatchButtonEvent("buttonUp")    
                         end
                       end,
  }
end

function Button:createToggleHandlerMap()
  return {
    mouseEnter = function(event) 
                    if not self:pushed() then      
                      self:state(Button.STATE_HOVER) 
                    end
                  end,
    mouseLeave = function(event)
                    if not self:pushed() then      
                      self:state(Button.STATE_NORMAL) 
                    end
                  end,
    mouseDown = function(event)
                    if not self:pushed() then      
                      self:state(Button.STATE_PUSHED)
                      self:dispatchButtonEvent("buttonDown")    
                    end
                  end,
    mouseUpInside = function(event)
                      if not self:pushed() then      
                        self:state(Button.STATE_PUSHED)
                      else
                        self:state(Button.STATE_HOVER)
                      end
                      self:pushed(not self:pushed())
                      self:dispatchButtonEvent("buttonClick")    
                      self:dispatchButtonEvent("buttonUp")    
                      end,
    mouseUpOutside = function(event)
                       if not self:pushed() then      
                         self:state(Button.STATE_NORMAL)
                         self:dispatchButtonEvent("buttonUp")    
                       end
                     end
  }
end

function Button:dispatchButtonEvent(name)
  local event = lost.guiro.event.Event(name)
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)  
end

function Button:state(newState)
  if newState ~= nil then
    self._state = newState
    -- FIXME: update display
  else
    return self._state
  end
end

function Button:enabled(enabled)
  if enabled ~= nil then
    self._enabled = enabled
    if self._enabled then
      self:state(Button.STATE_NORMAL)
    else
      self:state(Button.STATE_DISABLED)
    end
  else
    return not self._enabled
  end
end
