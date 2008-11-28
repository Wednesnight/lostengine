module("lost.guiro.controls", package.seeall)

require("lost.guiro.controls.Control")
require("lost.guiro.Bounds")

--[[
     Window control
  ]]
class "lost.guiro.controls.Window" (lost.guiro.controls.Control)
Window = _G["lost.guiro.controls.Window"]

lost.guiro.controls.Control:addBase(Window, "Window")

function Window:__init() super()
  self.header = {height = 25}

  self:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_MOVED, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self:handleInput(event) end)
  self:addEventListener(lost.application.TouchEvent.TOUCHES_CANCELLED, function(event) self:handleInput(event) end)
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
    if info.which == Control.InputType.down then
      local topmost = nil
      if self.parent then
        topmost = self.parent:getViewAt(info.location)
      end
      if (topmost == nil or rawequal(topmost, self)) and info.rect:contains(info.location) then
        if self.parent then
          self.parent:setFocus(self)
        end
        self.dragging = headerRect:contains(info.location)
        self.lastDragPos = info.location
      end

    -- release
    elseif info.which == Control.InputType.up then
      self.dragging = false

    -- move
    elseif info.which == Control.InputType.move then
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
