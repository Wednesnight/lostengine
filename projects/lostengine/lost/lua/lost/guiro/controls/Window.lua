module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

--[[
     Window control
  ]]
Window = lost.common.Class("lost.guiro.controls.Window", lost.guiro.View)

function Window:__init(properties)
  properties = properties or {}

  -- initialize defaults
  properties.backgroundColor = properties.backgroundColor or lost.common.Color(0.2,0.2,0.2)
  properties.borderColor = properties.borderColor or lost.common.Color(0.5,0.5,0.5)

  -- namespace alias
  local g = lost.guiro

  -- set header
  properties.header = properties.header or {}
  properties.header.bounds = properties.header.bounds or g.Bounds(g.xleft(), g.ytop(), g.wrel(1.0), g.habs(25))

  self.windowMouseDown = function(event) self:updateDragging(event, true) end
  self.windowMouseUp = function(event) self:updateDragging(event, false) end
  self.windowMouseUpOutside = function(event) self:updateDragging(event, false) end

  lost.guiro.View.__init(self, properties)
end

function Window:setProperty(key, value)
  -- namespace alias
  local g = lost.guiro

  if key == "header" then
    if self.header then
      self.header:removeEventListener(lost.guiro.event.MouseEvent.MOUSE_DOWN, self.windowMouseDown)
      self.header:removeEventListener(lost.guiro.event.MouseEvent.MOUSE_UP, self.windowMouseUp)
      self.header:removeEventListener(lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE, self.windowMouseUpOutside)
      self:removeChild(self.header)
    end
    self.header = g.controls.Label(value)
    self:appendChild(self.header)

    -- set header drag listeners
    self.header:addEventListener(lost.guiro.event.MouseEvent.MOUSE_DOWN, self.windowMouseDown)
    self.header:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP, self.windowMouseUp)
    self.header:addEventListener(lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE, self.windowMouseUpOutside)

    -- update content bounds
    if self.content then
      self.content.bounds = g.Bounds(g.xleft(), g.ybottom(), g.wrel(1.0), g.hrel(1.0, -self.header:globalRect().height))
    end

    return true
  
  elseif key == "content" then
    if self.content then
      self:removeChild(self.content)
    end
    self.content = g.View(value)
    self:appendChild(self.content)

    -- update content bounds
    if self.header then
      self.content.bounds = g.Bounds(g.xleft(), g.ybottom(), g.wrel(1.0), g.hrel(1.0, -self.header:globalRect().height))
    end

    return true
  end
  return false
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
