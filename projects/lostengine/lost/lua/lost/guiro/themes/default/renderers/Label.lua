module("lost.guiro.themes.default.renderer", package.seeall)

--[[
     Label Renderer
  ]]
require("lost.guiro.themes.Renderer")

class "lost.guiro.themes.default.renderer.Label" (lost.guiro.themes.Renderer)
Label = _G["lost.guiro.themes.default.renderer.Label"]

function Label:__init() super()
end

function Label:render(context, label, style)
	context:setColor(label.color)
	--context:drawRectFilled(label:globalRect())
	gl.glMatrixMode(gl.GL_MODELVIEW)
	gl.glLoadIdentity()
	local gr = label:globalRect()
	gl.glTranslate(gr.x, gr.y, 0)
	label.renderedText:render(context)
	gl.glLoadIdentity()
end
