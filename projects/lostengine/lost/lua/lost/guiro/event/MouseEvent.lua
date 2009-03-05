module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

MouseEvent = lost.common.Class("lost.guiro.event.MouseEvent",lost.guiro.event.Event)

MouseEvent.MOUSE_MOVE = "mouseMove"
MouseEvent.MOUSE_UP = "mouseUp"
MouseEvent.MOUSE_UP_OUTSIDE = "mouseUpOutside"
MouseEvent.MOUSE_DOWN = "mouseDown"
MouseEvent.MOUSE_ENTER = "mouseEnter"
MouseEvent.MOUSE_LEAVE = "mouseLeave"

local typeConvert = {}
typeConvert[lost.application.MouseEvent.MOUSE_MOVE] = MouseEvent.MOUSE_MOVE
typeConvert[lost.application.MouseEvent.MOUSE_UP] = MouseEvent.MOUSE_UP
typeConvert[lost.application.MouseEvent.MOUSE_DOWN] = MouseEvent.MOUSE_DOWN

-- initialise it with an uncast lost.application.MouseEvent
function MouseEvent:__init(laMouseEvent)
  lost.guiro.event.Event.__init(self)

  self.bubbles = true
  self.lostAppMouseEvent = lost.application.MouseEvent.cast(laMouseEvent)
  self.pos = self.lostAppMouseEvent.pos
  self.type = typeConvert[self.lostAppMouseEvent.type]
  self.stopDispatch = false
  self.stopPropagation = false
end
