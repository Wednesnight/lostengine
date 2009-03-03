module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Button Renderer
  ]]
Button = lost.common.Class("lost.guiro.themes.default.renderers.Button", lost.guiro.Renderer)

function Button:__init(properties) lost.guiro.Renderer.__init(self, properties)
  properties = properties or {}
end

function Button:render(canvas, button, style)
  lost.guiro.Renderer.render(self, canvas, button, button.style[button.state])
end
