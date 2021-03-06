module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollBar" "lost.guiro.view.View" {}

-- controls, set by theme
-- * slider
-- * incButton
-- * decButton
-- params set by theme
-- * buttonSize: required for slider handle size recalculation depending on visibleRange
--
function ScrollBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollbar"
  self.mode = t.mode or "horizontal"
  self:needsLayout()
  
  self.incButton:addEventListener("mouseDown", function(event) self:incButtonDown(event) end)
  self.incButton:addEventListener("mouseUp", function(event) self:incButtonUp(event) end)
  self.decButton:addEventListener("mouseDown", function(event) self:decButtonDown(event) end)
  self.decButton:addEventListener("mouseUp", function(event) self:decButtonUp(event) end)
  self.slider:addEventListener("mouseDown", function(event) self:sliderMouseDown(event) end)
  self.slider:addEventListener("mouseUp", function(event) self:sliderMouseUp(event) end)
  self.slider:addEventListener("valueChanged", function(event) self:sliderValueChanged(event) end)
  self._bbcounter = lost.profiler.BBCount("lost.guiro.view.ScrollBar")     
  self:minHandleSize(t.minHandleSize or 20)     
end

function ScrollBar:minHandleSize(...)
  if arg.n >= 1 then
    self._minHandleSize = arg[1]
    self:needsUpdate()
  else
    return self._minHandleSize
  end
end

function ScrollBar:updateLayout()
  lost.guiro.view.View.updateLayout(self)
  
  local hs = 0
  if self.mode == "horizontal" then
    hs = self.rect.width - 2*self.buttonSize
  else
    hs = self.rect.height - 2*self.buttonSize
  end  
  self.slider:handleSize(math.max(hs*self._visibleRange, self._minHandleSize))
end

function ScrollBar:visibleRange(...)
  if arg.n >= 1 then 
    self._visibleRange = arg[1]
    self:needsLayout()
  else
    return self._visibleRange
  end
end

function ScrollBar:value(...)
  if arg.n >= 1 then
    self.slider:value(arg[1])
  else
    return self.slider:value()
  end
end

function ScrollBar:stepSize(...)
  if arg.n >= 1 then
    self._stepSize = arg[1]
  else
    return self._stepSize
  end
end

function ScrollBar:sliderValueChanged(event)
  event.stopPropagation = true
  self:dispatchValueChangedEvent()
end

-- paging 
function ScrollBar:sliderMouseDown(event)
--  log.debug("slider "..event.type.." inHandle:"..tostring(self.slider:handleContainsPoint(event.pos)).." over:"..tostring(self.slider:pointOverHandle(event.pos)).." under:"..tostring(self.slider:pointUnderHandle(event.pos)))
  if not self.slider:handleContainsPoint(event.pos) then
    if self.slider:pointOverHandle(event.pos) then
      self.slider:value(self.slider:value()+self._visibleRange)
      self:dispatchValueChangedEvent()      
    elseif self.slider:pointUnderHandle(event.pos) then
      self.slider:value(self.slider:value()-self._visibleRange)
      self:dispatchValueChangedEvent()      
    end
  end
end

function ScrollBar:sliderMouseUp(event)
--  log.debug("slider "..event.type.." inHandle:"..tostring(self.slider:handleContainsPoint(event.pos)).." over:"..tostring(self.slider:pointOverHandle(event.pos)).." under:"..tostring(self.slider:pointUnderHandle(event.pos)))
end

function ScrollBar:incButtonDown(event)
--  log.debug("inc "..event.type)
  self.slider:value(self.slider:value()+self:stepSize())
  self:dispatchValueChangedEvent()
end

function ScrollBar:incButtonUp(event)
--log.debug("inc "..event.type)
end

function ScrollBar:decButtonDown(event)
--  log.debug("dec "..event.type)
  self.slider:value(self.slider:value()-self:stepSize())
  self:dispatchValueChangedEvent()
end

function ScrollBar:decButtonUp(event)
--log.debug("dec "..event.type)
end

function ScrollBar:dispatchValueChangedEvent()
  local event = lost.guiro.event.Event("valueChanged")
  event.bubbles = true
  event.target = self
  self:dispatchEvent(event)    
end
