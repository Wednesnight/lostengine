module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Image Renderer
  ]]
Image = lost.common.Class("lost.guiro.themes.default.renderers.Image", lost.guiro.Renderer)

function Image:__init(properties) lost.guiro.Renderer.__init(self, properties)
  properties = properties or {}
end

function Image:render(canvas, image, style) lost.guiro.Renderer.render(self, canvas, image, style)
end
