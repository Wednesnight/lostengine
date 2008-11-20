module("lost.guiro.controls", package.seeall)

--[[
     Window control
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Window" (lost.guiro.controls.Control)
Window = _G["lost.guiro.controls.Window"]

lost.guiro.controls.Control:addBase(Window, "Window")

function Window:__init() super()
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
  if info then
    local headerRect = lost.math.Rect(info.rect.x, 
                                      info.rect.y + (info.rect.height - self.style.header.height),
                                      info.rect.width,
                                      self.style.header.height)

    -- click
    if info.which == Control.InputType.down then
      self.dragging = headerRect:contains(info.location)
      self.lastDragPos = info.location
--      self.dragOffset = lost.math.Vec2(info.location.x - headerRect.x, info.location.y - headerRect.y)

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
        local newBounds = lost.math.Vec2(self.bounds.x + (info.location.x - self.lastDragPos.x),
                                         self.bounds.y + (info.location.y - self.lastDragPos.y))
        if parentRect then
          newBounds.x = math.max(math.min(newBounds.x, parentRect.width - self.bounds.width), 1)
          newBounds.y = math.max(math.min(newBounds.y, parentRect.height - self.bounds.height), 1)
        end

        self.bounds.x = newBounds.x
        self.bounds.y = newBounds.y
        self.lastDragPos = info.location
      end
    end
  end
end
