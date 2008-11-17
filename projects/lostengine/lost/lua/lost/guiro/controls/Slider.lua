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
  self.button:addEventListener(lost.guiro.controls.Button.ButtonPressed, function(event) self.lastDragPos = lost.math.Vec2(event.pos) self.dragging = true end)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonReleased, function(event) self.dragging = false end)
  self:appendChild(self.button)

  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleInput(event) end)
end

function Slider:handleInput(event)
  if self.dragging then
    local location = lost.math.Vec2(0,0)
    local globalRect = self:globalRect()
    if event.type == lost.application.MouseEvent.MOUSE_MOVE then
      local mouseEvent = lost.application.MouseEvent.cast(event)
      location = lost.math.Vec2(mouseEvent.pos)
    elseif event.type == lost.application.TouchEvent.TOUCHES_MOVED then
      local touchEvent = lost.application.TouchEvent.cast(event)
      if touchEvent:size() == 1 then
        local touch = touchEvent:get(0)
        location = lost.math.Vec2(touch.location)
      end
    end

    local delta = location - self.lastDragPos
    delta.x = self.button.bounds.x + delta.x
    if (delta.x >= 0 and delta.x <= self.bounds.width - self.button.bounds.width) then
      self.button.bounds.x = delta.x
      self.lastDragPos = location
    end
  end
end
