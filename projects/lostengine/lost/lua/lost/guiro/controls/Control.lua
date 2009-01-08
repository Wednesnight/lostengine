module("lost.guiro.controls", package.seeall)

--[[
     Control class
  ]]
require("lost.guiro.View")

class "lost.guiro.controls.Control" (lost.guiro.View)
Control = _G["lost.guiro.controls.Control"]

lost.guiro.View:addBase(Control, "Control")

function Control:__init() lost.guiro.View.__init(self)
end

Control.InputType =
{
  unknown = "unknown",
  down    = "down",
  up      = "up",
  move    = "move"
}

function Control:initializeInput(event)
  local validTypes = {[lost.application.MouseEvent.MOUSE_DOWN]        = true,
                      [lost.application.MouseEvent.MOUSE_UP]          = true,
                      [lost.application.MouseEvent.MOUSE_MOVE]        = true,
                      [lost.application.TouchEvent.TOUCHES_BEGAN]     = true,
                      [lost.application.TouchEvent.TOUCHES_ENDED]     = true,
                      [lost.application.TouchEvent.TOUCHES_CANCELLED] = true,
                      [lost.application.TouchEvent.TOUCHES_MOVED]     = true}

  -- valid event?
  if validTypes[event.type] then
    local info = {which    = Control.InputType.unknown,
                  location = lost.math.Vec2(0,0),
                  rect     = self:globalRect()}

    -- mouse event
    if event.type == lost.application.MouseEvent.MOUSE_DOWN or
       event.type == lost.application.MouseEvent.MOUSE_UP or
       event.type == lost.application.MouseEvent.MOUSE_MOVE
    then
      local mouseEvent = lost.application.MouseEvent.cast(event)
      info.location = lost.math.Vec2(mouseEvent.pos)
      if event.type == lost.application.MouseEvent.MOUSE_DOWN then
        info.which = Control.InputType.down
      elseif event.type == lost.application.MouseEvent.MOUSE_UP then
        info.which = Control.InputType.up
      elseif event.type == lost.application.MouseEvent.MOUSE_MOVE then
        info.which = Control.InputType.move
      end

    -- touch event
    elseif event.type == lost.application.TouchEvent.TOUCHES_BEGAN or
           event.type == lost.application.TouchEvent.TOUCHES_ENDED or
           event.type == lost.application.TouchEvent.TOUCHES_CANCELLED or
           event.type == lost.application.TouchEvent.TOUCHES_MOVED
    then
      local touchEvent = lost.application.TouchEvent.cast(event)
      if touchEvent:size() == 1 then
        local touch = touchEvent:get(0)
        info.location = lost.math.Vec2(touch.location)
        if event.type == lost.application.TouchEvent.TOUCHES_BEGAN then
          info.which = Control.InputType.down
        elseif event.type == lost.application.TouchEvent.TOUCHES_ENDED or
               event.type == lost.application.TouchEvent.TOUCHES_CANCELLED
        then
          info.which = Control.InputType.up
        elseif event.type == lost.application.TouchEvent.TOUCHES_MOVED then
          info.which = Control.InputType.move
        end
      end
    end
    return info
  end
  return false
end

function Control:redraw(context)
  if not self.theme then
    self.theme = lost.guiro.config.theme
  end
  if not self.renderer then
    self.renderer = self.theme.renderers[self:className()]()
  end
  if not self.style then
    self.style = self.theme.styles[self:className()]()
  end
  self.renderer:render(context, self, self.style)
end
