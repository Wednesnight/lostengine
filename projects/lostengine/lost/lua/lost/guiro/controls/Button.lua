module("lost.guiro.controls", package.seeall)

require("lost.guiro.controls.Control")

--[[
     Button class
  ]]
class "lost.guiro.controls.Button" (lost.guiro.controls.Control)
Button = _G["lost.guiro.controls.Button"]

function Button:__init() super()
  log.debug("Button:__init()")
end

function Button:__finalize()
  log.debug("Button:__finalize()")
end
