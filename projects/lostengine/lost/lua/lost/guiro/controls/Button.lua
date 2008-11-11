module("lost.guiro.controls", package.seeall)

--[[
     Button class
  ]]
require("lost.guiro.controls.Control")

class "lost.guiro.controls.Button" (lost.guiro.controls.Control)
Button = _G["lost.guiro.controls.Button"]

lost.guiro.controls.Control:addBase(Button, "Button")

function Button:__init() super()
  log.debug("Button:__init()")
end

function Button:render()
--  log.debug("Button:render()")
  lost.guiro.View.render(self)
end

function Button:__finalize()
  log.debug("Button:__finalize()")
end
