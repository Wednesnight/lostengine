module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Slider" "lost.guiro.view.View" {}

-- Slider will modify either x or y value of handle, depending on the selected mode
-- mandatory:
-- * handleReleasedLayer (must be set by theme)
-- * handlePushedLayer (must be set by theme)
-- * handleSize (must be set by theme) this is the width for horizontal or the height for vertical
-- optional
-- * mode: "horizontal" or "vertical", defaults to "horizontal"
-- * minValue: defaults to 0
-- * maxValue: defaults to 1
--
-- FIXME: no stepping yet, no automatic stepmarks yet
--
function Slider:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "slider"
  self.mode = t.mode or "horizontal"
  
  self.mouseDownHandler = function(event) self:mouseDown(event) end
  self.mouseUpHandler = function(event) self:mouseUp(event) end
  self.mouseMoveHandler = function(event) self:mouseMove(event) end
  self:addEventListener("mouseDown", self.mouseDownHandler)
  self:addEventListener("mouseUp", self.mouseUpHandler)
  self:addEventListener("mouseUpOutside", self.mouseUpHandler)
  self:addEventListener("mouseUpInside", self.mouseUpHandler)

  self:needsLayout()
  self:release()

  self.min = t.min or 0
  self.max = t.max or 1
  self.handleMouseOffset = 0 -- x offset of the mouse within the slider handle
  self.handlePos = 0 -- handle x position within the slider view
  self:value(self.min)
  self.dragInProgress = false
  self.scrollbarMode = false
end

function Slider:updateHandleLayerSizes()
  if self.mode == "horizontal" then
    self.handlePushedLayer:width(self._handleSize)
    self.handleReleasedLayer:width(self._handleSize)
  else
    self.handlePushedLayer:height(self._handleSize)
    self.handleReleasedLayer:height(self._handleSize)
  end
end

function Slider:handleSize(...)
  if arg.n >= 1 then
    self._handleSize = arg[1]
    self:updateHandleLayerSizes()
    self:needsLayout()
  else
    return self._handleSize
  end
end

-- sets current value, updates handle position accordingly
function Slider:value(...)
  if arg.n >= 1 then
    self._value = math.min(math.max(self.min, arg[1]), self.max)
    self:updateHandlePosFromValue()
    self:updateLayerPosFromHandlePos()
  else
    return self._value
  end
end

function Slider:updateHandlePosFromValue()
  self.handlePos = ((self._value - self.min)/self.max)*self:range()
end

function Slider:updateLayout()
  lost.guiro.view.View.updateLayout(self)
  self:updateHandlePosFromValue()
  self:updateLayerPosFromHandlePos()    
end

function Slider:mousePos(pos)
  if self.mode == "horizontal" then
    return pos.x
  else
    return pos.y
  end
end

function Slider:lowerHandleBound()
  if self.mode == "horizontal" then
    return self.handlePos+self.rect.x
  else
    return self.handlePos+self.rect.y
  end
end

function Slider:upperHandleBound()
  if self.mode == "horizontal" then
    return self.handlePos + self._handleSize + self.rect.x
  else
    return self.handlePos + self._handleSize + self.rect.y
  end
end

function Slider:rectMinPos()
  if self.mode == "horizontal" then
    return self.rect.x
  else
    return self.rect.y
  end
end

function Slider:upperHandleRangeBound()
  if self.mode == "horizontal" then
    return self.rect.width-self._handleSize
  else
    return self.rect.height-self._handleSize
  end
end

function Slider:range()
  if self.mode == "horizontal" then
    return self.rect.width - self._handleSize
  else
    return self.rect.height - self._handleSize
  end
end

-- calculates the initial x offset of the mouse within the handle once a mouseDown is registered
-- must lie within [0,width-handleSize]
function Slider:initHandleMouseOffset(currentGlobalPos)
  if (currentGlobalPos >= self:lowerHandleBound()) and (currentGlobalPos < self:upperHandleBound()) then
    -- mouse click was within handle, memorize the exact offset
    self.handleMouseOffset = currentGlobalPos - self:rectMinPos() - self.handlePos
  else
    -- mouse click was on track, so we set the offset to center of the handle
    self.handleMouseOffset = self._handleSize / 2
  end  
end

function Slider:updateLayerPosFromHandlePos()
  if self.mode == "horizontal" then
    self.handleReleasedLayer:x(self.handlePos)
    self.handlePushedLayer:x(self.handlePos)
  else
    self.handleReleasedLayer:y(self.handlePos)
    self.handlePushedLayer:y(self.handlePos)    
  end
end

function Slider:updateHandlePosFromMousePos(mousepos)
  self.handlePos = mousepos - self.handleMouseOffset - self:rectMinPos()
  -- clip the handlepos against the bounds of the view minus handlesize
  if self.handlePos < 0 then
    self.handlePos = 0
  elseif self.handlePos > self:upperHandleRangeBound() then
    self.handlePos = self:upperHandleRangeBound()
  end
  self:updateLayerPosFromHandlePos()
end

function Slider:updateValueFromHandlePos()
  local range = self:range()
  local v = self.handlePos / range
  self._value = self.min + v*(self.max - self.min)
end

function Slider:handleContainsPoint(point) -- vec2
  return self.handleReleasedLayer:containsPoint(point)
end

function Slider:pointOverHandle(point) -- "over" = top or right, depending on mode
  if self.mode == "horizontal" then
    return point.x >= (self.handleReleasedLayer.rect.x+self.handleReleasedLayer.rect.width)
  else
    return point.y >= (self.handleReleasedLayer.rect.y+self.handleReleasedLayer.rect.height)
  end
end

function Slider:pointUnderHandle(point) -- "under" = bottom or left, depending on mode
  if self.mode == "horizontal" then
    return point.x < self.handleReleasedLayer.rect.x
  else
    return point.y < self.handleReleasedLayer.rect.y
  end
end

function Slider:mouseDown(event)
  if (not self.scrollbarMode) or (self.scrollbarMode and self:handleContainsPoint(event.pos)) then
    self:rootview():addEventListener("mouseMove",self.mouseMoveHandler)
    self:initHandleMouseOffset(self:mousePos(event.pos))
    self:updateHandlePosFromMousePos(self:mousePos(event.pos))
    self:updateValueFromHandlePos()
    self:dispatchValueChangedEvent()
    self:press()
    self.dragInProgress = true
  end
end

function Slider:mouseUp(event)
  if self.dragInProgress then
    self:rootview():removeEventListener("mouseMove",self.mouseMoveHandler)
    self:release()
    self.dragInProgress = false
  end
end

function Slider:mouseMove(event)
  if self.dragInProgress then
    self:updateHandlePosFromMousePos(self:mousePos(event.pos))
    self:updateValueFromHandlePos()
    self:dispatchValueChangedEvent()  
  end
end

function Slider:press()
  self.handleReleasedLayer:hide()
  self.handlePushedLayer:show()
end

function Slider:release()
  self.handleReleasedLayer:show()
  self.handlePushedLayer:hide()
end

function Slider:dispatchValueChangedEvent()
  local event = lost.guiro.event.Event("valueChanged")
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)  
end

