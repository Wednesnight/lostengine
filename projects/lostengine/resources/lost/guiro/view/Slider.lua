module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

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
  
  
  self.mouseDownHandler = function(event) self:mouseDown(event) end
  self.mouseUpHandler = function(event) self:mouseUp(event) end
  self.mouseMoveHandler = function(event) self:mouseMove(event) end
  self:addEventListener("mouseDown", self.mouseDownHandler)
  self:addEventListener("mouseUp", self.mouseUpHandler)

  self:needsLayout()
  self:release()

  self.min = t.min or 0
  self.max = t.max or 1
  self.handleMouseOffset = 0 -- x offset of the mosue within the slider handle
  self.handlePos = 0 -- handle x position within the slider view
  self:value(self.min)
end

-- sets current value, updates handle position accordingly
function Slider:value(val)
  self.value = val
  -- FIXME: update handle position according to value
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
    return self.handlePos + self.handleSize + self.rect.x
  else
    return self.handlePos + self.handleSize + self.rect.y
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
    return self.rect.width-self.handleSize
  else
    return self.rect.height-self.handleSize
  end
end

function Slider:range()
  if self.mode == "horizontal" then
    return self.rect.width - self.handleSize
  else
    return self.rect.height - self.handleSize
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
    self.handleMouseOffset = self.handleSize / 2
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
  if self.mode == "horizontal" then
    self.handleReleasedLayer:x(self.handlePos)
    self.handlePushedLayer:x(self.handlePos)
  else
    self.handleReleasedLayer:y(self.handlePos)
    self.handlePushedLayer:y(self.handlePos)    
  end
end

function Slider:updateValueFromHandlePos()
  local range = self:range()
  local v = self.handlePos / range
  self.value = self.min + v*(self.max - self.min)
  log.debug(self.value)
end

function Slider:mouseDown(event)
  self:rootview():addEventListener("mouseMove",self.mouseMoveHandler)
  self:initHandleMouseOffset(self:mousePos(event.pos))
  self:updateHandlePosFromMousePos(self:mousePos(event.pos))
  self:updateValueFromHandlePos()
  self:press()
end

function Slider:mouseUp(event)
  self:rootview():removeEventListener("mouseMove",self.mouseMoveHandler)
  self:release()
end

function Slider:mouseMove(event)
  self:updateHandlePosFromMousePos(self:mousePos(event.pos))
  self:updateValueFromHandlePos()
end

function Slider:press()
  self.handleReleasedLayer:hide()
  self.handlePushedLayer:show()
end

function Slider:release()
  self.handleReleasedLayer:show()
  self.handlePushedLayer:hide()
end
