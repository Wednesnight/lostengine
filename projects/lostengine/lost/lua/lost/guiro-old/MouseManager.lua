module("lost.guiro", package.seeall) --MouseManager

require("lost.common.Class")
require("lost.guiro.event.MouseEvent")

lost.common.Class "lost.guiro.MouseManager" {}

function MouseManager:create(screen)
  log.debug("MouseManager:create")
  self.screen = screen
  self.currentMouse = nil
  
  self.screen:addEventListener(lost.guiro.event.MouseEvent.MOUSE_MOVE, function(event) self:updatePos(event.pos) end)
  self.currentPos = lost.math.Vec2()
end

function MouseManager:showOsMouse(visible)
  environment.application:showMouse(visible)
end

-- sets the custom mouse cursor to the given MouseCursor view object
-- nil is a valid input value, in which case the current cursor will be removed and none will be displayed
-- the native mouse will not be affected by this function and should be handled with showOsMouse
-- @return the old mouse cursor
function MouseManager:setMouse(newMouse)  
  local oldMouse = self.currentMouse
  if self.currentMouse then -- disable parent link to current screen, just in case
    self.currentMouse:setParent(nil)
  end
  self.currentMouse = newMouse  -- overwrite currentMouse with new value, nil is allowed
  if self.currentMouse then -- which is why we have to check here if the current mouse is valid
    self.currentMouse:setParent(self.screen)
    self.currentMouse:needsLayout()
    self.currentMouse:needsRedraw()
    self:updatePos(self.currentPos) -- sets the new mouseCursor to the position o fthe last event
  end
  return oldMouse
end

function MouseManager:updatePos(pos)
--  log.debug("updating mouse")
  self.currentPos = pos
  if self.currentMouse then
    self.currentMouse.bounds.x = lost.guiro.xabs(pos.x-self.currentMouse.hotspot.x)
    self.currentMouse.bounds.y = lost.guiro.yabs(pos.y-self.currentMouse.hotspot.y)
    self.currentMouse:needsLayout()
    self.currentMouse:needsRedraw()    
  end
end
