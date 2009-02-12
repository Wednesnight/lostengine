module("lost.guiro.Event", package.seeall)

require("lost.common.Object")

class "lost.guiro.Event" (lost.common.Object)
Event = _G["lost.guiro.Event"]

lost.guiro.Event:addBase(Event, "lost.guiro.Event");

lost.guiro.Event.EVENTPHASE_CAPTURE = 0
lost.guiro.Event.EVENTPHASE_TARGET = 1
lost.guiro.Event.EVENTPHASE_BUBBLE = 2

function Event:__init() lost.common.Object.__init(self)
  self.bubbles = false;
  self.
end