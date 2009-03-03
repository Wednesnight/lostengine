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
function Button:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  self.label = properties.label
  if self.label then
    self:appendChild(self.label)
  end

  self.state = properties.state or Button.State.released
  self.states = properties.states or {}

  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_DOWN, function(event) self:click(event, true) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP, function(event) self:click(event, false) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE, function(event) self:click(event, false) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_ENTER, function(event) self:hover(event, true) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_LEAVE, function(event) self:hover(event, false) end)
end

function Button:hover(event, hovered)
  self.hovered = hovered
  self:updateState()
end

function Button:click(event, clicked)
  self.down = clicked
  if self.down then
    self:dispatchEvent(Button.ButtonEvent(Button.ButtonPress, event.pos))
  else
    if self.hovered then
      self:dispatchEvent(Button.ButtonEvent(Button.ButtonClick, event.pos))
    end
    self:dispatchEvent(Button.ButtonEvent(Button.ButtonRelease, event.pos))
  end
  self:updateState()
end

function Button:updateState()
  self.pressed = self.down and self.hovered
  local oldState = self.state
  if self.pressed then
    self.state = Button.State.pressed
  elseif self.hovered then
    self.state = Button.State.hovered
  else
    self.state = Button.State.released
  end
  if oldState ~= self.state then
    self:needsRedraw()
  end
end
