module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.FocusEvent" "lost.guiro.event.Event" {}

FocusEvent.FOCUS_RECEIVED = lost.common.djb2Hash("focusReceived")
FocusEvent.FOCUS_LOST     = lost.common.djb2Hash("focusLost")

-- you may initialize it with the proper type
function FocusEvent:constructor(type)
  lost.guiro.event.Event.constructor(self,type)

  self.bubbles = true
--  self.type = type
  self.stopDispatch = false
  self.stopPropagation = false
end
