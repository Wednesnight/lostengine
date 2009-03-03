module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")
require("lost.guiro.controls.Button")
require("lost.guiro.event.Event")
require("lost.guiro.event.MouseEvent")

--[[
     Slider control
  ]]
Slider = lost.common.Class("lost.guiro.controls.Slider", lost.guiro.View)

--[[
    slider events
  ]]
Slider.SliderChange = "SLIDER_CHANGE"

Slider.SliderEvent = lost.common.Class("lost.guiro.controls.Slider.SliderEvent", lost.guiro.event.Event)

function Slider.SliderEvent:__init(which, target, value) lost.guiro.event.Event.__init(self, which)
  self.target = target
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
function Slider:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  self.orientation = properties.orientation or Slider.Orientation.horizontal
  self.min         = properties.min or 0
  self.max         = properties.max or 100
  self.steps       = properties.steps or 0

  if not properties.button then
    local g = lost.guiro
    local button = lost.guiro.controls.Button {
      id = "sliderButton",
      bounds = g.Bounds(g.xabs(1), g.yabs(1), g.wabs(24), g.habs(24))
    }
    self:setButton(button)
  else
    self:setButton(properties.button)
  end

  self.mouseMoved = function(event)
    if self.dragging then
      self:updatePosition(event.pos)
    end
  end

  if properties.value then
    self:value(properties.value, true)
  end  
end

function Slider:setButton(newButton)
  self:removeChild(self.button)
  self.button = newButton
  self:appendChild(self.button)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonPress, function(event) self:toggleDragging(true) end)
  self.button:addEventListener(lost.guiro.controls.Button.ButtonRelease, function(event) self:toggleDragging(false) end)
end

function Slider:toggleDragging(dragging)
  self.dragging = dragging
  if self.dragging then
    self:screen():addEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, self.mouseMoved)
  else
    self:screen():removeEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, self.mouseMoved)
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
    local position = math.max(math.min(location[coord] - globalRect[coord] - buttonRect[size] / 2, (globalRect[size] - buttonRect[size])), 0)
    local stepSize = ((globalRect[size] - buttonRect[size]) / math.abs(self.max - self.min)) * (math.abs(self.max - self.min) / self.steps)
    local steps, fraction = math.modf(position / stepSize)
    if (fraction > 0.5) then
      steps = steps + 1
    end
    position = math.max(math.min(steps * stepSize, (globalRect[size] - buttonRect[size])), 0)
    if (position ~= buttonRect[coord]) then
      self.button.bounds[coord] = coordFunc(position)
      self.button:needsLayout()
    end
  else
    self.button.bounds[coord] = coordFunc(math.max(math.min(location[coord] - globalRect[coord], (globalRect[size] - buttonRect[size])), 0))
    self.button:needsLayout()
  end
  local newValue = self:value()
  if (oldValue ~= newValue) then
    self:needsRedraw()
    if not silent then
      self:dispatchEvent(Slider.SliderEvent(Slider.SliderChange, self, newValue))
    end
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
    buttonRect[coord] = (((localRect[size] - buttonRect[size]) / math.abs(self.max - self.min)) * value) + globalRect[coord]
    self:updatePosition(lost.math.Vec2(buttonRect.x, buttonRect.y), silent)
  end

  buttonRect = self.button:localRect()
  return (buttonRect[coord] / ((localRect[size] - buttonRect[size]) / math.abs(self.max - self.min))) + self.min
end
