module("lost.guiro.controls", package.seeall)

require("lost.guiro.controls.Control")

--[[
     Window class
  ]]
class "lost.guiro.controls.Window" (lost.guiro.controls.Control)
Window = _G["lost.guiro.controls.Window"]

function Window:__init() super()
  log.debug("Window:__init()")
end

function Window:__finalize()
  log.debug("Window:__finalize()")
end
