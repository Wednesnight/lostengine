module("lost.guiro.controls", package.seeall)

--[[
     Button class
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Button" (lost.guiro.controls.Control)
Button = _G["lost.guiro.controls.Button"]

lost.guiro.controls.Control:addBase(Button, "Button")

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

    -- left button release
    elseif mouseEvent.type == lost.application.MouseEvent.MOUSE_UP then
      self.down = false
      self.pressed = self.down

    -- left button release
    elseif mouseEvent.type == lost.application.MouseEvent.MOUSE_MOVE then
      self.hovered = globalRect:contains(mouseEvent.pos)
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
