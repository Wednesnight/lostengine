module("lost.guiro", package.seeall)

--[[
     Screen class
  ]]
require("lost.guiro.View")

class "lost.guiro.Screen" (lost.guiro.View)
Screen = _G["lost.guiro.Screen"]

function Screen:__init() super()
  log.debug("Screen:__init()")
end

function Screen:render()
  log.debug("Screen:render()")
  lost.guiro.View.render(self)
end

function Screen:__finalize()
  log.debug("Screen:__finalize()")
end
