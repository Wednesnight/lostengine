-- lost.guiro.Button
module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Label")
require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.Button" "lost.guiro.View" {}

using "lost.guiro.event.Event"

Button.STATE_NORMAL = "normal"
Button.STATE_HOVER = "hover"
Button.STATE_PUSHED = "pushed"
Button.STATE_DISABLED = "disabled"

function Button:constructor(textureManager)
	lost.guiro.View.constructor(self, textureManager)

	self._state = Button.STATE_NORMAL
  self._allStates = {Button.STATE_NORMAL, Button.STATE_HOVER, Button.STATE_PUSHED, Button.STATE_DISABLED}
	self._states = {}
	self._states[Button.STATE_NORMAL] = {}
	self._states[Button.STATE_HOVER] = {}
	self._states[Button.STATE_PUSHED] = {}
	self._states[Button.STATE_DISABLED] = {}
	self._enabled = true
	self._title = ""
	
  self:addEventListener("mouseEnter", function(event) self:mouseEnter(event) end)	
  self:addEventListener("mouseLeave", function(event) self:mouseLeave(event) end)	
  self:addEventListener("mouseDown", function(event) self:mouseDown(event) end)	
  self:addEventListener("mouseUpInside", function(event) self:mouseUpInside(event) end)	
  self:addEventListener("mouseUpOutside", function(event) self:mouseUpOutside(event) end)	
end

function Button:mouseEnter(event)
  if self._enabled then
    self._state = Button.STATE_HOVER
    callLater(self.updateViewVisibility, self)
  end
end

function Button:mouseLeave(event)
  if self._enabled then
    self._state = Button.STATE_NORMAL
    callLater(self.updateViewVisibility, self)
  end
end

function Button:mouseDown(event)
  if self._enabled then
    self._state = Button.STATE_PUSHED
    callLater(self.updateViewVisibility, self)
    local pressEvent = lost.guiro.event.Event("buttonDown")
    pressEvent.bubbles = true
    pressEvent.target = self
    self:dispatchEvent(pressEvent)
  end
end

function Button:mouseUpInside(event)
  if self._enabled then
    self._state = Button.STATE_HOVER
    callLater(self.updateViewVisibility, self)
    local clickEvent = lost.guiro.event.Event("buttonClick")
    clickEvent.bubbles = true
    clickEvent.target = self
    self:dispatchEvent(clickEvent)
    local releaseEvent = lost.guiro.event.Event("buttonUp")
    releaseEvent.bubbles = true
    releaseEvent.target = self
    self:dispatchEvent(releaseEvent)
  end
end

function Button:mouseUpOutside(event)
  if self._enabled then
    self._state = Button.STATE_NORMAL
    callLater(self.updateViewVisibility, self)
    local releaseEvent = lost.guiro.event.Event("buttonUp")
    releaseEvent.bubbles = true
    releaseEvent.target = self
    self:dispatchEvent(releaseEvent)
  end
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
      if bg then bg:hidden(false) end
      if l then l:hidden(false) end
    else
      if bg then bg:hidden(true) end
      if l then l:hidden(true) end
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
  callLater(self.updateViewOrder, self)
  callLater(self.updateViewVisibility, self)
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
  callLater(self.updateViewOrder, self)
  callLater(self.updateViewVisibility, self)
end

function Button:state(newState)
  if newState ~= nil then
    self._state = newState
    callLater(self.updateViewVisibility, self)
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