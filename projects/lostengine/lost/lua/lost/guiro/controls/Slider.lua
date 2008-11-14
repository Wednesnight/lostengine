module("lost.guiro.controls", package.seeall)

--[[
     Slider control
  ]]
require("lost.guiro.controls.Control")
require("lost.guiro.controls.Button")

class "lost.guiro.controls.Slider" (lost.guiro.controls.Control)
Slider = _G["lost.guiro.controls.Slider"]

lost.guiro.controls.Control:addBase(Slider, "Slider")

--[[
    constructor
  ]]
function Slider:__init() super()
  self.bounds = lost.math.Rect(0,0,100,25)

  self.button = lost.guiro.controls.Button()
  self.button.id = "sliderButton"
  self.button.bounds = lost.math.Rect(0,0,25,25)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonPressed, function(event) self.lastDragPos = event.pos self.dragging = true end)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonReleased, function(event) self.dragging = false end)
  self:appendChild(self.button)

  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleMouse(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleTouch(event) end)
end

function Slider:handleMouse(event)
  if self.dragging then
    local mouseEvent = lost.application.MouseEvent.cast(event)
    local globalRect = self:globalRect()

    local delta = mouseEvent.pos - self.lastDragPos
    delta.x = self.button.bounds.x + delta.x
    if (delta.x >= 0 and delta.x <= self.bounds.width - self.button.bounds.width) then
      self.button.bounds.x = delta.x
      self.lastDragPos = mouseEvent.pos
    end
  end
end

function Slider:handleTouch(event)
  if self.dragging then
    local touchEvent = lost.application.TouchEvent.cast(event)
    local globalRect = self:globalRect()
    local touch = touchEvent:get(0)
    local location = lost.math.Vec2(touch.location)

    local delta = location - self.lastDragPos
    delta.x = self.button.bounds.x + delta.x
    if (delta.x >= 0 and delta.x <= self.bounds.width - self.button.bounds.width) then
      self.button.bounds.x = delta.x
      self.lastDragPos = location
    end
  end
end
