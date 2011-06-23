module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.MouseEvent" "lost.guiro.event.Event" {}

MouseEvent.MOUSE_MOVE = lost.common.djb2Hash("mouseMove")
MouseEvent.MOUSE_UP = lost.common.djb2Hash("mouseUp")
MouseEvent.MOUSE_UP_INSIDE = lost.common.djb2Hash("mouseUpInside")
MouseEvent.MOUSE_UP_OUTSIDE = lost.common.djb2Hash("mouseUpOutside")
MouseEvent.MOUSE_DOWN = lost.common.djb2Hash("mouseDown")
MouseEvent.MOUSE_ENTER = lost.common.djb2Hash("mouseEnter")
MouseEvent.MOUSE_LEAVE = lost.common.djb2Hash("mouseLeave")
MouseEvent.MOUSE_SCROLL = lost.common.djb2Hash("mouseScroll")

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
