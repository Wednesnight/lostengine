module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.event.Event")

--[[
     Button control
  ]]
Button = lost.common.Class("lost.guiro.controls.Button", lost.guiro.View)

--[[
    button event types
  ]]
Button.ButtonPress   = "BUTTON_PRESS"
Button.ButtonRelease = "BUTTON_RELEASE"
Button.ButtonClick   = "BUTTON_CLICK"

Button.ButtonEvent = lost.common.Class("lost.guiro.controls.Button.ButtonEvent", lost.guiro.event.Event)

function Button.ButtonEvent:__init(which, pos) lost.guiro.event.Event.__init(self, which)
  self.pos = pos
end

--[[
    button states
  ]]
Button.State =
{
  released = "released",
  hovered  = "hovered",
  pressed  = "pressed"
}

--[[
    constructor
  ]]
function Button:__init(properties)
  properties = properties or {}
  
  -- initialize defaults
  properties.released = properties.released or {}
  properties.released.backgroundColor = properties.released.backgroundColor or lost.common.Color(0.5,0.5,0.5,1)
  properties.released.fontColor = properties.released.fontColor or lost.common.Color(1,1,1,1)

  properties.hovered = properties.hovered or {}
  properties.hovered.backgroundColor = properties.hovered.backgroundColor or lost.common.Color(0.6,0.6,0.6,1)
  properties.hovered.fontColor = properties.hovered.fontColor or lost.common.Color(1,1,1,1)

  properties.pressed = properties.pressed or {}
  properties.pressed.backgroundColor = properties.pressed.backgroundColor or lost.common.Color(0.3,0.3,0.3,1)
  properties.pressed.fontColor = properties.pressed.fontColor or lost.common.Color(1,1,1,1)

  properties.label = lost.guiro.controls.Label(properties.label)

  lost.guiro.View.__init(self, properties)

  -- initialize state
  self:updateState()

  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_DOWN, function(event) self:click(event, true) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP, function(event) self:click(event, false) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE, function(event) self:click(event, false) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_ENTER, function(event) self:hover(event, true) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_LEAVE, function(event) self:hover(event, false) end)
end

function Button:setProperty(key, value)
  if key == "label" then
    if self.label then
      self:removeChild(self.label)
    end
    self.label = value
    self:appendChild(self.label)
    return true
  end
  return false
end

function Button:hover(event, hovered)
  self.isHovered = hovered
  self:updateState()
end

function Button:click(event, clicked)
  self.isDown = clicked
  if self.isDown then
    self:dispatchEvent(Button.ButtonEvent(Button.ButtonPress, event.pos))
  else
    if self.isHovered then
      self:dispatchEvent(Button.ButtonEvent(Button.ButtonClick, event.pos))
    end
    self:dispatchEvent(Button.ButtonEvent(Button.ButtonRelease, event.pos))
  end
  self:updateState()
end

function Button:updateState()
  self.isPressed = self.isDown and self.isHovered
  local oldState = self.state
  if self.isPressed then
    self.state = Button.State.pressed
  elseif self.isHovered then
    self.state = Button.State.hovered
  else
    self.state = Button.State.released
  end
  if oldState ~= self.state then
    self:set(self[self.state])
  end
end
