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
Button.ButtonPressed  = "BUTTON_PRESSED"
Button.ButtonReleased = "BUTTON_RELEASED"
Button.ButtonClicked  = "BUTTON_CLICKED"

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
  
  self:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:handleMouse(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:handleMouse(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleMouse(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self:handleTouch(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleTouch(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self:handleTouch(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, function(event) self:handleTouch(event) end)
end

--[[
    mouse handler for click/hover
    sets self.state
  ]]
function Button:handleMouse(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)
  local globalRect = self:globalRect()

  if mouseEvent.button == lost.application.MB_LEFT or
     mouseEvent.type == lost.application.MouseEvent.MOUSE_MOVE
  then
    -- left button click
    if mouseEvent.type == lost.application.MouseEvent.MOUSE_DOWN then
      self.down = globalRect:contains(mouseEvent.pos)
      self.pressed = self.down
      if self.down then
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonPressed, mouseEvent.pos))
      end

    -- left button release
    elseif mouseEvent.type == lost.application.MouseEvent.MOUSE_UP then
      self.down = false
      self.pressed = false

    -- move
    elseif mouseEvent.type == lost.application.MouseEvent.MOUSE_MOVE then
      self.hovered = globalRect:contains(mouseEvent.pos)
      self.pressed = self.down and self.hovered
      if not self.down then
        if self.hovered then
          self:dispatchEvent(Button.ButtonEvent(Button.ButtonClicked, mouseEvent.pos))
        end
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonReleased, mouseEvent.pos))
      end
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

--[[
    touch handler for click/hover
    sets self.state
  ]]
function Button:handleTouch(event)
  local touchEvent = lost.application.TouchEvent.cast(event)
  local globalRect = self:globalRect()

  if touchEvent:size() == 1 then
    local touch = touchEvent:get(0)
    local location = lost.math.Vec2(touch.location)

    -- touch
    if touchEvent.type == lost.application.TouchEvent.TOUCHES_BEGAN then
      self.down = globalRect:contains(location)
      self.pressed = self.down
      if self.down then
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonPressed, location))
      end

    -- touch release
    elseif touchEvent.type == lost.application.TouchEvent.TOUCHES_ENDED or
           touchEvent.type == lost.application.TouchEvent.TOUCHES_CANCELLED
    then
      self.down = false
      self.pressed = false
      self.hovered = false

    -- move
    elseif touchEvent.type == lost.application.TouchEvent.TOUCHES_MOVED then
      self.hovered = globalRect:contains(location)
      self.pressed = self.down and self.hovered
      if not self.down then
        if self.hovered then
          self:dispatchEvent(Button.ButtonEvent(Button.ButtonClicked, location))
        end
        self:dispatchEvent(Button.ButtonEvent(Button.ButtonReleased, location))
      end
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
