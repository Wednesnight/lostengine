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
  if info then
    local headerRect = lost.math.Rect(info.rect.x, 
                                      info.rect.y + (info.rect.height - self.header.height),
                                      info.rect.width,
                                      self.header.height)

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
        local globalRect = self:globalRect()
        local parentRect = nil
        if self.parent then
          parentRect = self.parent:globalRect()
        end
        local newBounds = lost.math.Vec2(globalRect.x + (info.location.x - self.lastDragPos.x),
                                         globalRect.y + (info.location.y - self.lastDragPos.y))
        if parentRect then
          newBounds.x = math.max(math.min(newBounds.x, parentRect.width - globalRect.width), 1)
          newBounds.y = math.max(math.min(newBounds.y, parentRect.height - globalRect.height), 1)
        end

        self.bounds.x = lost.guiro.xabs(newBounds.x)
        self.bounds.y = lost.guiro.yabs(newBounds.y)
        self.lastDragPos = info.location
      end
    end
  end
end
