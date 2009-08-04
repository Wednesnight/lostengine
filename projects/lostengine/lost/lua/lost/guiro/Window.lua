module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

--[[
     Window control
  ]]
lost.common.Class "lost.guiro.Window" "lost.guiro.View" {}

function Window:constructor()
  -- namespace alias
  local g = lost.guiro

  self.windowMouseDown = function(event) self:updateDragging(event, true) end
  self.windowMouseUp = function(event) self:updateDragging(event, false) end
  self.windowMouseUpOutside = function(event) self:updateDragging(event, false) end

  lost.guiro.View.constructor(self)
end

function Window:updateDragging(event, dragging)
  self.dragging = dragging
  -- update listeners
  if self.dragging then
    self.lastDragPos = event.pos
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
