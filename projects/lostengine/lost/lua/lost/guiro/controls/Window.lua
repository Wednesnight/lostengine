module("lost.guiro.controls", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

--[[
     Window control
  ]]
Window = lost.common.Class("lost.guiro.controls.Window", lost.guiro.View)

function Window:__init() lost.guiro.View.__init(self)
  self.header = {height = 25}
end

--[[
    input handler for dragging
  ]]
function Window:handleInput(event)
  local info = self:initializeInput(event)
  if info and (self.dragging or info.rect:contains(info.location)) then
    local headerRect = lost.math.Rect(info.rect.x, 
                                      info.rect.y + (info.rect.height - self.header.height),
                                      info.rect.width,
                                      self.header.height)

    -- click
    if info.which == View.InputType.down then
      local topmost = nil
      if self.parent then
        topmost = self.parent:getViewAt(info.location)
      end
      if (topmost == nil or rawequal(topmost, self)) and info.rect:contains(info.location) then
        self.dragging = headerRect:contains(info.location)
        self.lastDragPos = info.location
      end

    -- release
    elseif info.which == View.InputType.up then
      self.dragging = false

    -- move
    elseif info.which == View.InputType.move then
      if self.dragging then
        local parentRect = nil
        if self.parent then
          parentRect = self.parent:globalRect()
        end
        local newBounds = lost.math.Vec2(info.rect.x + (info.location.x - self.lastDragPos.x),
                                         info.rect.y + (info.location.y - self.lastDragPos.y))
        if parentRect then
          newBounds.x = math.max(math.min(newBounds.x, parentRect.width - info.rect.width), 1)
          newBounds.y = math.max(math.min(newBounds.y, parentRect.height - info.rect.height), 1)
        end

        self.bounds.x = lost.guiro.xabs(newBounds.x)
        self.bounds.y = lost.guiro.yabs(newBounds.y)
        self:needsLayout()
        self.lastDragPos = info.location
      end
    end
  end
end

function Window:needsRedraw()
  lost.guiro.View.needsRedraw(self)
  if self.parent then
    self.parent.dirty = true
  end
end
