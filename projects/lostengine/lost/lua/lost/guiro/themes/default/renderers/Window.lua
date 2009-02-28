module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Window Renderer
  ]]
Window = lost.common.Class("lost.guiro.themes.default.renderers.Window", lost.guiro.Renderer)

function Window:__init(properties) lost.guiro.Renderer.__init(self, properties)
  properties = properties or {}
end

function Window:render(canvas, window, style) lost.guiro.Renderer.render(self, canvas, window, style)
end
