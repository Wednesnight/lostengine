module("lost.guiro.controls", package.seeall)

--[[
     Control class
  ]]
require("lost.guiro.View")

class "lost.guiro.controls.Control" (lost.guiro.View)
Control = _G["lost.guiro.controls.Control"]

function Control:__init() super()
  log.debug("Control:__init()")
  self.cache = {}
end

function Control:render()
  log.debug("Control:render()")
  lost.guiro.View.render(self)
end

function Control:__finalize()
  log.debug("Control:__finalize()")
  self.cache = {}
end
