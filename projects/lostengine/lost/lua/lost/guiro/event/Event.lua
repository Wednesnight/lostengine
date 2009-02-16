module("lost.guiro.event", package.seeall) -- Event

require("lost.common.Object")

Event = lost.common.Class("lost.guiro.event.Event", lost.common.Object)

lost.guiro.event.Event.PHASE_CAPTURE = 0
lost.guiro.event.Event.PHASE_TARGET = 1
lost.guiro.event.Event.PHASE_BUBBLE = 2

function Event:__init(inType) lost.common.Object.__init(self)
  self.type = inType                                -- identifies the event type
  self.bubbles = false                              -- set this to true if the event should bubble
  self.stopDispatch = false                         -- set this to false if you want to stop the dispatching of this event
  self.stopPropagation = false                      -- set this to false if you want to stop the propagation of this event
  self.phase = lost.guiro.event.Event.PHASE_TARGET  -- default phase is target, other phases will be set by the EventManager 
  self.target = nil                                 -- the target of an event (e.g. the lowermost view of a mouseDown)
  self.currentTarget = nil                          -- the current event target during propagtion 
end

function Event:__tostring()
  return self.type
end