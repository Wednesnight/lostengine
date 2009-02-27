module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

--[[
     Window control
  ]]
Window = lost.common.Class("lost.guiro.controls.Window", lost.guiro.View)

function Window:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  self.header = properties.header or
  {
    height = 25
  }
  
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_DOWN, function(event) self:updateDragging(event, true) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP, function(event) self:updateDragging(event, false) end)
  self:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE, function(event) self:updateDragging(event, false) end)
end

function Window:updateDragging(event, dragging)
  self.dragging = dragging

  --[[
    check header bounds
  ]]
  if dragging then
    local rect = self:globalRect()
    local headerRect = lost.math.Rect(rect.x, 
                                      rect.y + (rect.height - self.header.height),
                                      rect.width,
                                      self.header.height)
    if not headerRect:contains(event.pos) then self.dragging = false
      else self.lastDragPos = event.pos end
  end

  --[[
    update listeners
  ]]
  if self.dragging then
    self:screen():addEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, function(event) self:updatePosition(event) end)
  else
    self:screen():removeEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, function(event) self:updatePosition(event) end)
  end
end

--[[
    dragging
  ]]
function Window:updatePosition(event)
  if self.dragging then
    local rect = self:globalRect()
    local parentRect = nil
    if self.parent then
      parentRect = self.parent:globalRect()
    end
    local newBounds = lost.math.Vec2(rect.x + (event.pos.x - self.lastDragPos.x),
                                     rect.y + (event.pos.y - self.lastDragPos.y))
    if parentRect then
      newBounds.x = math.max(math.min(newBounds.x, parentRect.width - rect.width), 0)
      newBounds.y = math.max(math.min(newBounds.y, parentRect.height - rect.height), 0)
    end

    self.bounds.x = lost.guiro.xabs(newBounds.x)
    self.bounds.y = lost.guiro.yabs(newBounds.y)
    self:needsLayout()
    self.lastDragPos = event.pos
  end
end

function Window:needsRedraw()
  lost.guiro.View.needsRedraw(self)
  if self.parent then
    self.parent.dirty = true
  end
end
