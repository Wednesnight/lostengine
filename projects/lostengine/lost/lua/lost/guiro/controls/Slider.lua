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
    slider events
  ]]
Slider.SliderChange = "SLIDER_CHANGE"

class "lost.guiro.controls.Slider.SliderEvent" (lost.event.Event)
Slider.SliderEvent = _G["lost.guiro.controls.Slider.SliderEvent"]

function Slider.SliderEvent:__init(which, value) super(which)
  self.value = value
end

--[[
    slider orientation
  ]]
Slider.Orientation =
{
  horizontal = "horizontal",
  vertical   = "vertical"
}

--[[
    constructor
  ]]
function Slider:__init() super()
  self.orientation = Slider.Orientation.horizontal
  self.min         = 0
  self.max         = 100
  self.steps       = 0

  self.bounds = lost.math.Rect(0,0,100,25)

  self.button = lost.guiro.controls.Button()
  self.button.id = "sliderButton"
  self.button.bounds = lost.math.Rect(0,0,25,25)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonPress, function(event) self.dragging = true end)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonRelease, function(event) self.dragging = false end)
  self:appendChild(self.button)

  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleInput(event) end)
end

function Slider:handleInput(event)
  if self.dragging then
    local location = lost.math.Vec2(0,0)
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

    self:updatePosition(location)
  end
end

function Slider:updatePosition(location)
  local globalRect = self:globalRect()
  local coord = "x"
  local size  = "width"
  if (self.orientation == Slider.Orientation.vertical) then
    coord = "y"
    size  = "height"
  end

  local oldValue = self:value()
  if (self.steps > 0) then
    local position = math.max(math.min(location[coord] - globalRect[coord] - self.button.bounds[size] / 2, (self.bounds[size] - self.button.bounds[size])), 0)
    local stepSize = ((self.bounds[size] - self.button.bounds[size]) / (self.max - self.min)) * ((self.max - self.min) / self.steps)
    local steps, fraction = math.modf(position / stepSize)
    if (fraction > 0.75) then
      steps = steps + 1
    end
    position = math.max(math.min(steps * stepSize, (self.bounds[size] - self.button.bounds[size])), 0)
    if (position ~= self.button.bounds[coord]) then
      self.button.bounds[coord] = position
    end
  else
    self.button.bounds[coord] = math.max(math.min(location[coord] - globalRect[coord], (self.bounds[size] - self.button.bounds[size])), 0)
  end
  local newValue = self:value()
  if (oldValue ~= newValue) then
    self:dispatchEvent(Slider.SliderEvent(Slider.SliderChange, newValue))
  end
end

function Slider:value(value)
  local globalRect = self:globalRect()
  local coord = "x"
  local size  = "width"
  if (self.orientation == Slider.Orientation.vertical) then
    coord = "y"
    size  = "height"
  end

  if value ~= nil then
    self.button.bounds[coord] = ((self.bounds[size] - self.button.bounds[size]) / math.abs(self.max - self.min)) * value
    self:updatePosition(lost.math.Vec2(self.button.bounds.x + globalRect.x, self.button.bounds.y + globalRect.y))
  end

  return (self.button.bounds[coord] / ((self.bounds[size] - self.button.bounds[size]) / (self.max - self.min))) + self.min
end
