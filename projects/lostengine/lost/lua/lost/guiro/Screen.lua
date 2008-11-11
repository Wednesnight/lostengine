module("lost.guiro", package.seeall)

--[[
     Screen class
  ]]
require("lost.guiro.View")

class "lost.guiro.Screen" (lost.guiro.View)
Screen = _G["lost.guiro.Screen"]

lost.guiro.View:addBase(Screen, "Screen")

function Screen:__init() super()
  log.debug("Screen:__init()")
end

function Screen:dispatchEvent(event)
  log.debug("Screen:dispatchEvent()")
  lost.guiro.View.dispatchEvent(self, event)
end

function Screen:render()
--  log.debug("Screen:render()")
  lost.guiro.View.render(self)
end

function Screen:__finalize()
  log.debug("Screen:__finalize()")
end
