module("lost.guiro.view", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.view.Button" "lost.guiro.view.View" {}

local Event = lost.guiro.event.Event

Button.STATE_NORMAL = "normal"  
Button.STATE_HOVER = "hover" 
Button.STATE_PUSHED = "pushed"  
Button.STATE_DISABLED = "disabled"
Button.STATE_NORMAL2 = "normal2"  
Button.STATE_HOVER2 = "hover2" 
Button.STATE_PUSHED2 = "pushed2"  

Button.BUTTON_DOWN = lost.common.djb2Hash("buttonDown")
Button.BUTTON_UP = lost.common.djb2Hash("buttonUp")
Button.BUTTON_CLICK = lost.common.djb2Hash("buttonClick")

local me = lost.application.MouseEvent

function Button:constructor(args)
  local t = args or {}
  self.titles = {} -- one string per state
  self.titleColors = {} -- color for title per state
  self.backgrounds = {} -- each state has a dedicated layer
  self.textLayer = nil -- only one layer that holds the current title
  self._allStates = {Button.STATE_NORMAL, Button.STATE_HOVER, Button.STATE_PUSHED, Button.STATE_DISABLED,
                     Button.STATE_NORMAL2, Button.STATE_HOVER2, Button.STATE_PUSHED2}
	self:state(Button.STATE_NORMAL)
	self._enabled = true
  self._pushed = false;
  self._handlerMaps = {}
  self._handlerMaps["normal"] = self:createNormalHandlerMap()
  self._handlerMaps["sticky"] = self:createStickyHandlerMap()
  self._handlerMaps["toggle"] = self:createToggleHandlerMap()
  self._handlerMaps["toggleOnce"] = self:createToggleOnceHandlerMap()
  self._currentHandlerMap = nil
	self:mode(t.mode or "normal")

	lost.guiro.view.View.constructor(self, args) -- call after aall members that are required for style are setup

  self.id = t.id or "button"

  if t.title then self:title(t.title) end
	if t.pushed then self:pushed(t.pushed) end
	
  self:addEventListener("mouseEnter", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseLeave", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseDown", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseUpInside", function(event) self:eventHandler(event) end)	
  self:addEventListener("mouseUpOutside", function(event) self:eventHandler(event) end)	
  self:needsLayout()
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.Button")      
end

function Button:pushed(...)
  if arg.n >= 1 then
    local val = arg[1]
    if val then 
      self._pushed = true
    else
      self._pushed = false
    end
    if self._pushed then
      if (self._mode == "toggle") or (self._mode == "toggleOnce") then
        self:togglestate(Button.STATE_NORMAL)
      else  
        self:state(Button.STATE_PUSHED)
      end
    else
      self:state(Button.STATE_NORMAL)
    end
    self:needsDisplay()
  else
    return self._pushed
  end
end

function Button:mode(modeName)
  self._mode = modeName
  self._currentHandlerMap = self._handlerMaps[modeName]
  if self._currentHandlerMap == nil then
    log.warn("couldn't find handlerMap for mode: '"..tostring(modeName).."', defaulting to normal")
    self._currentHandlerMap = self._handlerMaps["normal"]
    self._mode = "normal"
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
    [me.MOUSE_ENTER] = function(event)
                  if not self.mouseIsDown then
                    self:state(Button.STATE_HOVER) 
                  else
                    self:state(Button.STATE_PUSHED)
                  end
                  end,
    [me.MOUSE_LEAVE] = function(event)
                  self:state(Button.STATE_NORMAL) 
                  end,
    [me.MOUSE_DOWN] = function(event)
                  self.mouseIsDown = true
                  self:state(Button.STATE_PUSHED)
                  self:dispatchButtonEvent(Button.BUTTON_DOWN)    
                  end,
    [me.MOUSE_UP_INSIDE] = function(event)
                      self.mouseIsDown = false
                      self:state(Button.STATE_HOVER)
                      self:dispatchButtonEvent(Button.BUTTON_CLICK)    
                      self:dispatchButtonEvent(Button.BUTTON_UP)    
                      end,
    [me.MOUSE_UP_OUTSIDE] = function(event)
                       self.mouseIsDown = false
                       self:state(Button.STATE_NORMAL)
                       self:dispatchButtonEvent(Button.BUTTON_UP)    
                       end,
  }
end

function Button:createStickyHandlerMap()
  return {
    [me.MOUSE_ENTER] = function(event) 
                    if not self:pushed()  then
                      if not self.mouseIsDown then
                        self:state(Button.STATE_HOVER)                       
                      else
                        self:state(Button.STATE_PUSHED)
                      end
                    end
                  end,
    [me.MOUSE_LEAVE] = function(event)
                    if not self:pushed() then
                      self:state(Button.STATE_NORMAL) 
                    end
                  end,
    [me.MOUSE_DOWN] = function(event)
                    if not self:pushed() then
                      self.mouseIsDown = true
                      self:state(Button.STATE_PUSHED)
                      self:dispatchButtonEvent(Button.BUTTON_DOWN)    
                    end
                  end,
    [me.MOUSE_UP_INSIDE] = function(event)
                        if not self:pushed() then   
                          self.mouseIsDown = false                           
                          self:pushed(true)
                          self:state(Button.STATE_PUSHED)
                          self:dispatchButtonEvent(Button.BUTTON_CLICK)
                        end
                      end,
    [me.MOUSE_UP_OUTSIDE] = function(event)
                         if not self:pushed() then      
                           self.mouseIsDown = false
                           self:state(Button.STATE_NORMAL)
                           self:dispatchButtonEvent(Button.BUTTON_UP)    
                         end
                       end,
  }
end

function Button:createToggleHandlerMap()
  return {
    [me.MOUSE_ENTER] = function(event) 
                      if not self.mouseIsDown then
                        self:togglestate(Button.STATE_HOVER)                       
                      else
                        self:togglestate(Button.STATE_PUSHED) 
                      end
                  end,
    [me.MOUSE_LEAVE] = function(event)
                  self:togglestate(Button.STATE_NORMAL) 
                  end,
    [me.MOUSE_DOWN] = function(event)
                      self.mouseIsDown = true                       
                      self:togglestate(Button.STATE_PUSHED)
                      self:dispatchButtonEvent(Button.BUTTON_DOWN)    
                  end,
    [me.MOUSE_UP_INSIDE] = function(event)
                      self.mouseIsDown = false
                      self:pushed(not self:pushed())
                      self:togglestate(Button.STATE_HOVER)
                      self:dispatchButtonEvent(Button.BUTTON_CLICK)    
                      self:dispatchButtonEvent(Button.BUTTON_UP)    
                      end,
    [me.MOUSE_UP_OUTSIDE] = function(event)
                       self.mouseIsDown = false
                       self:togglestate(Button.STATE_NORMAL)
                       self:dispatchButtonEvent(Button.BUTTON_UP)    
                     end
  }
end

function Button:createToggleOnceHandlerMap()
  return {
    [me.MOUSE_ENTER] = function(event)
                    if self:pushed() then return end
                    if not self.mouseIsDown then
                      self:togglestate(Button.STATE_HOVER)                       
                    else
                      self:togglestate(Button.STATE_PUSHED) 
                    end
                  end,
    [me.MOUSE_LEAVE] = function(event)
                    if self:pushed() then return end
                    self:togglestate(Button.STATE_NORMAL) 
                  end,
    [me.MOUSE_DOWN] = function(event)
                    if self:pushed() then return end
                    self.mouseIsDown = true                       
                    self:togglestate(Button.STATE_PUSHED)
                    self:dispatchButtonEvent(Button.BUTTON_DOWN)    
                  end,
    [me.MOUSE_UP_INSIDE] = function(event)
                        if self:pushed() then return end
                        self.mouseIsDown = false
                        self:pushed(not self:pushed())
                        self:togglestate(Button.STATE_HOVER)
                        self:dispatchButtonEvent(Button.BUTTON_CLICK)    
                        self:dispatchButtonEvent(Button.BUTTON_UP)    
                      end,
    [me.MOUSE_UP_OUTSIDE] = function(event)
                       if self:pushed() then return end
                       self.mouseIsDown = false
                       self:togglestate(Button.STATE_NORMAL)
                       self:dispatchButtonEvent(Button.BUTTON_UP)    
                     end
  }
end

-- use this function like you would use state. IT will set the state
-- depending on the value(), i.e. if you set STATE_NORMAL and 
-- pushed is true, STATE_NORMAL2 will be set
function Button:togglestate(newstate)
  if self:pushed() then
    if newstate == Button.STATE_NORMAL then self:state(Button.STATE_NORMAL2) 
    elseif newstate == Button.STATE_HOVER then self:state(Button.STATE_HOVER2) 
    elseif newstate == Button.STATE_PUSHED then self:state(Button.STATE_PUSHED2) 
    else self:state(newstate) end
  else
    self:state(newstate)
  end
end

function Button:dispatchButtonEvent(name)
  local event = lost.guiro.event.Event(name)
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)  
end

function Button:updateDisplay()
  lost.guiro.view.View.updateDisplay(self)
  -- show active background layer or normal by default
  local current = self.backgrounds[self._state]
  if not current then
    if self._state == Button.STATE_HOVER then current = self.backgrounds[Button.STATE_NORMAL] 
    elseif self._state == Button.STATE_HOVER2 then current = self.backgrounds[Button.STATE_NORMAL2] 
    else current = nil end
  end
  if not current then
    return
  end
  for _,v in pairs(self.backgrounds) do
    if not rawequal(current,v) then
      v:hidden(true)
    else
      v:hidden(false)
    end
  end
  -- update title for state, default to normal title or nil, will only be set if there is a textLayer
  local t = self.titles[self._state]
  if not t then t = self.titles[Button.STATE_NORMAL] end
  if not t then t = "" end
  if t and self.textLayer then
    self.textLayer:text(t)
  end
  -- update color for state
  local c = self.titleColors[self._state]
  if not c then c = self.titleColors[Button.STATE_NORMAL] end
  if c and self.textLayer then
    self.textLayer:color(c)
  end
end

function Button:state(newState)
  if newState ~= nil and (newState ~= self._state) then
--    log.debug("-- "..newState)
    self._state = newState
    self:needsDisplay()
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

-- either one param = text for all states => will only be put into normal and reused
-- or state/title pair => will only be set for exactly this state
function Button:title(...)
  if arg.n == 0 then
    for k,v in pairs(self._allStates) do
      return self.titles[v]
    end
  elseif arg.n == 1 then
    for k,v in pairs(self._allStates) do
      self.titles[v] = nil
    end
    self.titles[Button.STATE_NORMAL] = arg[1]
    self:needsDisplay()
  elseif arg.n == 2 then
    self.titles[arg[1]] = arg[2]
    self:needsDisplay()
  end
end
