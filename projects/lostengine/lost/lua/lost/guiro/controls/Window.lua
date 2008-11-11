module("lost.guiro.controls", package.seeall)

--[[
     Window class
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Window" (lost.guiro.controls.Control)
Window = _G["lost.guiro.controls.Window"]

lost.guiro.controls.Control:addBase(Window, "Window")

function Window:__init() super()
  log.debug("Window:__init()")
end

function Window:render()
--  log.debug("Window:render()")
  lost.guiro.View.render(self)
end

function Window:__finalize()
  log.debug("Window:__finalize()")
end
