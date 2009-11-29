module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.MouseEvent" "lost.guiro.event.Event" {}

MouseEvent.MOUSE_MOVE = "mouseMove"
MouseEvent.MOUSE_UP = "mouseUp"
MouseEvent.MOUSE_UP_INSIDE = "mouseUpInside"
MouseEvent.MOUSE_UP_OUTSIDE = "mouseUpOutside"
MouseEvent.MOUSE_DOWN = "mouseDown"
MouseEvent.MOUSE_ENTER = "mouseEnter"
MouseEvent.MOUSE_LEAVE = "mouseLeave"
MouseEvent.MOUSE_SCROLL = "mouseScroll"

local typeConvert = {}
typeConvert[lost.application.MouseEvent.MOUSE_MOVE] = MouseEvent.MOUSE_MOVE
typeConvert[lost.application.MouseEvent.MOUSE_UP] = MouseEvent.MOUSE_UP
typeConvert[lost.application.MouseEvent.MOUSE_DOWN] = MouseEvent.MOUSE_DOWN
typeConvert[lost.application.MouseEvent.MOUSE_SCROLL] = MouseEvent.MOUSE_SCROLL

-- initialise it with an lost.application.MouseEvent
function MouseEvent:constructor(laMouseEvent)
  lost.guiro.event.Event.constructor(self)

  local lostAppMouseEvent = laMouseEvent

  self.bubbles = true
  self.pos = lostAppMouseEvent.pos
  self.absPos = lostAppMouseEvent.absPos
  self.button = lostAppMouseEvent.button
  self.pressed = lostAppMouseEvent.pressed
  self.scrollDelta = lostAppMouseEvent.scrollDelta
  self.type = typeConvert[lostAppMouseEvent.type]
  self.stopDispatch = false
  self.stopPropagation = false
end
