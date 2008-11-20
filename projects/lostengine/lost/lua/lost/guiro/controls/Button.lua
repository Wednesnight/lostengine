module("lost.guiro.controls", package.seeall)

--[[
     Button control
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Button" (lost.guiro.controls.Control)
Button = _G["lost.guiro.controls.Button"]

lost.guiro.controls.Control:addBase(Button, "Button")

--[[
    button events
  ]]
Button.ButtonPress   = "BUTTON_PRESS"
Button.ButtonRelease = "BUTTON_RELEASE"
Button.ButtonClick   = "BUTTON_CLICK"
Button.ButtonEnter   = "BUTTON_ENTER"
Button.ButtonLeave   = "BUTTON_LEAVE"

class "lost.guiro.controls.Button.ButtonEvent" (lost.event.Event)
Button.ButtonEvent = _G["lost.guiro.controls.Button.ButtonEvent"]

function Button.ButtonEvent:__init(which, pos) super(which)
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
function Button:__init() super()
  self.state = Button.State.released
  
  self:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, function(event) self:handleInput(event) end)
end

--[[
    input handler for click/hover
    sets self.state
  ]]
function Button:handleInput(event)
  local info = self:initializeInput(event)
  if info then
    -- click
    if info.which == Control.InputType.down then
      self.down = info.rect:contains(info.location)
      self.pressed = self.down
      if self.down then
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonPress, info.location))
      end

    -- release
    elseif info.which == Control.InputType.up then
      if self.down then
        if self.hovered then
          self:dispatchEvent(Button.ButtonEvent(Button.ButtonClick, info.location))
        end
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonRelease, info.location))
      end
      self.down = false
      self.pressed = false
      self.hovered = info.rect:contains(info.location)

    -- move
    elseif info.which == Control.InputType.move then
      local wasHovered = self.hovered
      self.hovered = info.rect:contains(info.location)
      if not wasHovered and self.hovered then
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonEnter, info.location))
      elseif wasHovered and not self.hovered then
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonLeave, info.location))
      end
      self.pressed = self.down and self.hovered
    end
    
    if self.pressed then
      self.state = Button.State.pressed
    elseif self.hovered then
      self.state = Button.State.hovered
    else
      self.state = Button.State.released
    end
  end
end
