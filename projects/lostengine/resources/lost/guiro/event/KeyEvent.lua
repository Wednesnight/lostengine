module("lost.guiro.event", package.seeall)

require("lost.guiro.event.Event")

lost.common.Class "lost.guiro.event.KeyEvent" "lost.guiro.event.Event" {}

KeyEvent.KEY_UP   = "keyUp"
KeyEvent.KEY_DOWN = "keyDown"

local typeConvert = {}
typeConvert[lost.application.KeyEvent.KEY_UP] = KeyEvent.KEY_UP
typeConvert[lost.application.KeyEvent.KEY_DOWN] = KeyEvent.KEY_DOWN

-- initialise it with an lost.application.KeyEvent
function KeyEvent:constructor(laKeyEvent)
  lost.guiro.event.Event.constructor(self)

  self.bubbles = true
  self.lostAppKeyEvent = laKeyEvent
  self.key = self.lostAppKeyEvent.key
  self.character = self.lostAppKeyEvent.character
  self.pressed = self.lostAppKeyEvent.pressed
  self.ctrlDown = self.lostAppKeyEvent.ctrlDown
  self.shiftDown = self.lostAppKeyEvent.shiftDown
  self.altDown = self.lostAppKeyEvent.altDown
  self.specialDown = self.lostAppKeyEvent.specialDown
  self.repeat_ = self.lostAppKeyEvent.repeat_
  self.type = typeConvert[self.lostAppKeyEvent.type]
  self.stopDispatch = false
  self.stopPropagation = false
end
