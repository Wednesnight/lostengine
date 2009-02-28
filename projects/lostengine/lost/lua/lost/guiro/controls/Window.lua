module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

--[[
     Window control
  ]]
Window = lost.common.Class("lost.guiro.controls.Window", lost.guiro.View)

function Window:__init(properties) lost.guiro.View.__init(self, properties)
  properties = properties or {}

  -- namespace alias
  local g = lost.guiro

  -- set header
  properties.header = properties.header or {}
  properties.header.bounds = properties.header.bounds or g.Bounds(g.xleft(), g.ytop(), g.wrel(1.0), g.habs(25))
  self.header = g.controls.Label(properties.header)
  self:appendChild(self.header)

  -- set header drag listeners
  self.header:addEventListener(lost.guiro.event.MouseEvent.MOUSE_DOWN, function(event) self:updateDragging(event, true) end)
  self.header:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP, function(event) self:updateDragging(event, false) end)
  self.header:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE, function(event) self:updateDragging(event, false) end)

  -- set content view
  properties.content = properties.content or {}
  properties.content.bounds = g.Bounds(g.xleft(), g.ybottom(), g.wrel(1.0), g.hrel(1.0, -self.header:globalRect().height))
  self.content = g.View(properties.content)
  self:appendChild(self.content)
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
