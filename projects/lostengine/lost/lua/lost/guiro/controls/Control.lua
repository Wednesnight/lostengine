module("lost.guiro.controls", package.seeall)

--[[
     Control class
  ]]
class "lost.guiro.controls.Control" (lost.guiro.View)
Control = _G["lost.guiro.controls.Control"]

function Control:__init() super()
  log.debug("Control:__init()")
end

function Control:render()
  log.debug("Control:render()")
  lost.guiro.View.render(self)
end

function Control:__finalize()
  log.debug("Control:__finalize()")
end
