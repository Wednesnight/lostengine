module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.FocusEvent" "lost.guiro.event.Event" {}

FocusEvent.FOCUS_RECEIVED = "focusReceived"
FocusEvent.FOCUS_LOST     = "focusLost"

-- you may initialize it with the proper type
function FocusEvent:create(type)
  lost.guiro.event.Event.create(self)

  self.bubbles = true
  self.type = type
  self.stopDispatch = false
  self.stopPropagation = false
end
