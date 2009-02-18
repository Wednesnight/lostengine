module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")
require("lost.guiro.controls.Button")
require("lost.guiro.event.Event")

--[[
     Slider control
  ]]
Slider = lost.common.Class("lost.guiro.controls.Slider", lost.guiro.View)

--[[
    slider events
  ]]
Slider.SliderChange = "SLIDER_CHANGE"

Slider.SliderEvent = lost.common.Class("lost.guiro.controls.Slider.SliderEvent", lost.guiro.event.Event)

function Slider.SliderEvent:__init(which, value) lost.guiro.event.Event.__init(self, which)
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
function Slider:__init() lost.guiro.View.__init(self)
  self.orientation = Slider.Orientation.horizontal
  self.min         = 0
  self.max         = 100
  self.steps       = 0

  local g = lost.guiro
  local button = lost.guiro.controls.Button()
  button.id = "sliderButton"
  button.bounds = g.Bounds(g.xabs(0), g.yabs(0), g.wabs(24), g.habs(24))
  button.fadeStates = true
  self:setButton(button)

  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleInput(event) end)
end

function Slider:setButton(newButton)
  self:removeChild(self.button)
  self.button = newButton
  self:appendChild(self.button)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonPress, function(event) self.dragging = true end)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonRelease, function(event) self.dragging = false end)
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

function Slider:updatePosition(location, silent)
  local coord = "x"
  local coordFunc = lost.guiro.xabs
  local size  = "width"
  if (self.orientation == Slider.Orientation.vertical) then
    coord = "y"
    coordFunc = lost.guiro.yabs
    size  = "height"
  end

  local oldValue = self:value()
  local globalRect = self:globalRect()
  local buttonRect = self.button:localRect()
  if (self.steps > 0) then
    local position = math.max(math.min(location[coord] - globalRect[coord] - buttonRect[size] / 2, (globalRect[size] - buttonRect[size] - 1)), 0)
    local stepSize = ((globalRect[size] - buttonRect[size] - 1) / math.abs(self.max - self.min)) * (math.abs(self.max - self.min) / self.steps)
    local steps, fraction = math.modf(position / stepSize)
    if (fraction > 0.5) then
      steps = steps + 1
    end
    position = math.max(math.min(steps * stepSize, (globalRect[size] - buttonRect[size] - 1)), 0)
    if (position ~= buttonRect[coord]) then
      self.button.bounds[coord] = coordFunc(position)
      self.button:needsLayout()
    end
  else
    self.button.bounds[coord] = coordFunc(math.max(math.min(location[coord] - globalRect[coord], (globalRect[size] - buttonRect[size] - 1)), 0))
    self.button:needsLayout()
  end
  local newValue = self:value()
  if not silent and (oldValue ~= newValue) then
    self:dispatchEvent(Slider.SliderEvent(Slider.SliderChange, newValue))
  end
end

function Slider:value(value, silent)
  local localRect = self:localRect()
  local globalRect = self:globalRect()
  local buttonRect = self.button:globalRect()
  local coord = "x"
  local coordFunc = lost.guiro.xabs
  local size  = "width"
  if (self.orientation == Slider.Orientation.vertical) then
    coord = "y"
    coordFunc = lost.guiro.yabs
    size  = "height"
  end

  if value ~= nil then
    buttonRect[coord] = (((localRect[size] - buttonRect[size] - 1) / math.abs(self.max - self.min)) * value) + globalRect[coord]
    self:updatePosition(lost.math.Vec2(buttonRect.x, buttonRect.y), silent)
  end

  buttonRect = self.button:localRect()
  return (buttonRect[coord] / ((localRect[size] - buttonRect[size] - 1) / math.abs(self.max - self.min))) + self.min
end
