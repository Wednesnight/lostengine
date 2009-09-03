-- lost.guiro.Button
module("lost.guiro", package.seeall)

require("lost.guiro.Label")
require("lost.guiro.Image")

lost.common.Class "lost.guiro.Button" "lost.guiro.View" {}

Button.STATE_NORMAL = "normal"
Button.STATE_HOVER = "hover"
Button.STATE_PUSHED = "pushed"
Button.STATE_DISABLED = "disabled"

function Button:constructor()
	lost.guiro.View.constructor(self)
	self._state = Button.STATE_NORMAL
  self._allStates = {Button.STATE_NORMAL, Button.STATE_HOVER, Button.STATE_PUSHED, Button.STATE_DISABLED}
	self._states = {}
	self._states[Button.STATE_NORMAL] = {}
	self._states[Button.STATE_HOVER] = {}
	self._states[Button.STATE_PUSHED] = {}
	self._states[Button.STATE_DISABLED] = {}
	self._disabled = false
	self._title = ""
	self.deferredUpdateViewOrder = function() 
	  self:updateViewOrder()
	end
	
	self.deferredUpdateViewVisibility = function()
    self:updateViewVisibility()
	end
	
  self:addEventListener("mouseEnter", function(event) self:mouseEnter(event) end)	
  self:addEventListener("mouseLeave", function(event) self:mouseLeave(event) end)	
  self:addEventListener("mouseDown", function(event) self:mouseDown(event) end)	
  self:addEventListener("mouseUp", function(event) self:mouseUp(event) end)	
  self:addEventListener("mouseUpOutside", function(event) self:mouseUpOutside(event) end)	
end

function Button:mouseEnter(event)
  self._state = Button.STATE_HOVER
  callLater(self.deferredUpdateViewVisibility)  
end

function Button:mouseLeave(event)
  self._state = Button.STATE_NORMAL
  callLater(self.deferredUpdateViewVisibility)  
end

function Button:mouseDown(event)
  self._state = Button.STATE_PUSHED
  callLater(self.deferredUpdateViewVisibility)  
end

function Button:mouseUp(event)
  self._state = Button.STATE_HOVER
  callLater(self.deferredUpdateViewVisibility)  
  local clickEvent = lost.guiro.event.Event("buttonClick")
  clickEvent.bubbles = true
  clickEvent.target = self
  self:bubbleEvent(clickEvent)
end

function Button:mouseUpOutside(event)
  self._state = Button.STATE_NORMAL
  callLater(self.deferredUpdateViewVisibility)  
end


-- makes sure backgrounds are in the back and labels are in front
function Button:updateViewOrder()
  self:removeAllSubviews()
  for k,stateName in pairs(self._allStates) do
    local bg = self._states[stateName].background
    local label = self._states[stateName].label
    if bg then
      self:addSubview(bg)
      bg:needsLayout()
    end
    if label then 
      self:addSubview(label)
      label:needsLayout()
    end
  end
end

-- call this after the state has changed or the control was created to 
-- toggle visibility flags accoridng to the current state
function Button:updateViewVisibility()
  for k,stateName in pairs(self._allStates) do
    local bg = self._states[stateName].background
    local l = self._states[stateName].label
    if stateName == self._state then
      if bg then bg:hidden(false) else log.debug("bg is nil") end
      if l then l:hidden(false)   else log.debug("label is nil") end
    else
      if bg then bg:hidden(true) else log.debug("bg is nil")  end
      if l then l:hidden(true)  else log.debug("label is nil") end
    end
  end
end

-- removes old view and memorizes new view in state tables, but deferrs ordering of view
-- until all states are set
function Button:background(stateName, view)
  if (stateName == nil) or (type(stateName) ~= "string") then
    error("Button:background : stateName is mandatory and must be a string, got: '"..tostring(stateName).."'")
  end
  local oldView = self._states[stateName].background
  if oldView then
    self:removeSubview(oldView)
  end
  self._states[stateName].background = view
  callLater(self.deferredUpdateViewOrder)
  callLater(self.deferredUpdateViewVisibility)
end

function Button:label(stateName, view)
  if (stateName == nil) or (type(stateName) ~= "string") then
    error("Button:label : stateName is mandatory and must be a string, got: '"..tostring(stateName).."'")
  end
  local oldView = self._states[stateName].label
  if oldView then
    self:removeSubview(oldView)
  end
  self._states[stateName].label = view
  callLater(self.deferredUpdateViewOrder)
  callLater(self.deferredUpdateViewVisibility)
end

function Button:state(newState)
  if newState ~= nil then
    self._state = newState
    callLater(self.deferredUpdateViewVisibility)
  else
    return self._state
  end
end

function Button:title(txt)
  if txt ~= nil then
    for k,stateName in pairs(self._allStates) do
      local l = self._states[stateName].label
      if l then l:text(txt) end
    end
  else
    return self._title
  end
end
