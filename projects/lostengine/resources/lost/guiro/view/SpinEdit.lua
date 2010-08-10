-- lost.guiro.SpinEdit
module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.view.SpinEdit" "lost.guiro.view.View" {}

function SpinEdit:constructor(textureManager)
  lost.guiro.view.View.constructor(self, textureManager)
  
  self._value = 0
  self._min = 0
  self._max = 10
  self._stepSize = 1

  self._label = nil
  self._downButton = nil
  self._upButton = nil
end

function SpinEdit:value(value)
  if value ~= nil then
    local correctedValue = value - math.fmod(value, self._stepSize)
    if correctedValue ~= self._value and correctedValue >= self._min and correctedValue <= self._max then
      self._value = correctedValue
      self._label:text(tostring(self._value))

      local valueChangedEvent = lost.guiro.event.Event("valueChanged")
      valueChangedEvent.bubbles = true
      valueChangedEvent.target = self
      valueChangedEvent.value = self._value
      self:dispatchEvent(valueChangedEvent)
    end
  else
    return self._value
  end
end

function SpinEdit:min(min)
  if min ~= nil then
    self._min = min
    self:value(math.max(self._value, self._min))
  else
    return self._min
  end
end

function SpinEdit:max(max)
  if max ~= nil then
    self._max = max
    self:value(math.min(self._value, self._max))
  else
    return self._max
  end
end

function SpinEdit:stepSize(stepSize)
  if stepSize ~= nil then
    self._stepSize = stepSize
  else
    return self._stepSize
  end
end

function SpinEdit:label(label)
  if label ~= nil then
    self._label = label
    self._label:text(tostring(self._value))
    self:addSubview(self._label)
  else
    return self._label
  end
end

function SpinEdit:downButton(downButton)
  if downButton ~= nil then
    self._downButton = downButton
    self._downButton:addEventListener("buttonClick", function(event)
      if event.currentTarget == event.target then
        self:value(self._value - self._stepSize)
      end
    end)
    self:addSubview(self._downButton)
  else
    return self._downButton
  end
end

function SpinEdit:upButton(upButton)
  if upButton ~= nil then
    self._upButton = upButton
    self._upButton:addEventListener("buttonClick", function(event)
      if event.currentTarget == event.target then
        self:value(self._value + self._stepSize)
      end
    end)
    self:addSubview(self._upButton)
  else
    return self._upButton
  end
end
