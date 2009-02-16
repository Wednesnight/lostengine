module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

ViewEvent = lost.common.Class("lost.guiro.event.ViewEvent",lost.guiro.event.Event)

ViewEvent.MOUSE_ENTER = "viewMouseEnter"
ViewEvent.MOUSE_LEAVE = "viewMouseLeave"

-- initialise it with an uncast lost.application.MouseEvent
function ViewEvent:__init(inType) lost.guiro.event.Event.__init(self, inType)
  self.bubbles = true
end