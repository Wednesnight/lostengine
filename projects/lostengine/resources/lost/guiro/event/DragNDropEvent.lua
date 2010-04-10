module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.DragNDropEvent" "lost.guiro.event.Event"
{
  DRAG_ENTER  = lost.application.DragNDropEvent.DRAG_ENTER,
  DRAG_UPDATE = lost.application.DragNDropEvent.DRAG_UPDATE,
  DRAG_LEAVE  = lost.application.DragNDropEvent.DRAG_LEAVE,
  DROP        = lost.application.DragNDropEvent.DROP
}

-- initialise it with an lost.application.DragNDropEvent
function DragNDropEvent:constructor(laDragNDropEvent)
  lost.guiro.event.Event.constructor(self)

  self.bubbles = true
  self.lostAppDragNDropEvent = laDragNDropEvent
  self.filename = self.lostAppDragNDropEvent.filename
  self.window = self.lostAppDragNDropEvent.window
  self.pos = self.lostAppDragNDropEvent.pos
  self.absPos = self.lostAppDragNDropEvent.absPos
  self.type = self.lostAppDragNDropEvent.type
  self.stopDispatch = false
  self.stopPropagation = false
end
