module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.DropEvent" "lost.guiro.event.Event" {}

DropEvent.DROPPED_FILE   = "droppedFile"

local typeConvert = {}
typeConvert[lost.application.DropEvent.DROPPED_FILE] = DropEvent.DROPPED_FILE

-- initialise it with an lost.application.DropEvent
function DropEvent:constructor(laDropEvent)
  lost.guiro.event.Event.constructor(self)

  self.bubbles = true
  self.lostAppDropEvent = laDropEvent
  self.filename = self.lostAppDropEvent.filename
  self.window = self.lostAppDropEvent.window
  self.pos = self.lostAppDropEvent.pos
  self.absPos = self.lostAppDropEvent.absPos
  self.type = typeConvert[self.lostAppDropEvent.type]
  self.stopDispatch = false
  self.stopPropagation = false
end
