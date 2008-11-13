module("lost.guiro.controls", package.seeall)

--[[
     Window class
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Window" (lost.guiro.controls.Control)
Window = _G["lost.guiro.controls.Window"]

lost.guiro.controls.Control:addBase(Window, "Window")

function Window:__init() super()
  self:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:handleMouse(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:handleMouse(event) end)
  self:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:handleMouse(event) end)
end

--[[
    mouse handler for dragging
  ]]
function Window:handleMouse(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)
  local globalRect = self:globalRect()
  local headerRect = lost.math.Rect(globalRect.x, 
                                    globalRect.y + (globalRect.height - self.style.header.height),
                                    globalRect.width,
                                    self.style.header.height)

  if mouseEvent.button == lost.application.MB_LEFT or
     mouseEvent.type == lost.application.MouseEvent.MOUSE_MOVE
  then
    -- left button click
    if mouseEvent.type == lost.application.MouseEvent.MOUSE_DOWN then
      self.dragging = headerRect:contains(mouseEvent.pos)
      self.lastDragPos = mouseEvent.pos

    -- left button release
    elseif mouseEvent.type == lost.application.MouseEvent.MOUSE_UP then
      self.dragging = false

    -- left button release
    elseif mouseEvent.type == lost.application.MouseEvent.MOUSE_MOVE then
      if self.dragging then
        self.bounds.x = self.bounds.x + (mouseEvent.pos.x - self.lastDragPos.x)
        self.bounds.y = self.bounds.y + (mouseEvent.pos.y - self.lastDragPos.y)
        self.lastDragPos = mouseEvent.pos
      end
    end
  end
end
