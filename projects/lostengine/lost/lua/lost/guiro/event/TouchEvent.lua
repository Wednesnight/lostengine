module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.TouchEvent" "lost.guiro.event.Event" {}

TouchEvent.TOUCHES_BEGAN = "touchesBegan"
TouchEvent.TOUCHES_CANCELLED = "touchesCancelled"
TouchEvent.TOUCHES_ENDED = "touchesEnded"
TouchEvent.TOUCHES_MOVED = "touchesMoved"

local typeConvert = {}
typeConvert[lost.application.TouchEvent.TOUCHES_BEGAN] = TouchEvent.TOUCHES_BEGAN
typeConvert[lost.application.TouchEvent.TOUCHES_CANCELLED] = TouchEvent.TOUCHES_CANCELLED
typeConvert[lost.application.TouchEvent.TOUCHES_ENDED] = TouchEvent.TOUCHES_ENDED
typeConvert[lost.application.TouchEvent.TOUCHES_MOVED] = TouchEvent.TOUCHES_MOVED

-- initialise it with an uncast lost.application.TouchEvent
function TouchEvent:constructor(laTouchEvent)
  lost.guiro.event.Event.constructor(self)

  local lostAppTouchEvent = lost.application.TouchEvent.cast(laTouchEvent)

  self.bubbles = true
--  self.pos = lostAppTouchEvent.pos
  self.type = typeConvert[lostAppTouchEvent.type]
  self.size = lostAppTouchEvent:size()
  self.touches = lostAppTouchEvent.touches
  self.stopDispatch = false
  self.stopPropagation = false
end
