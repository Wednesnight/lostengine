module("lost.guiro.themes.default.renderers", package.seeall)

--[[
     Label Renderer
  ]]
require("lost.guiro.Renderer")

class "lost.guiro.themes.default.renderers.Label" (lost.guiro.Renderer)
Label = _G["lost.guiro.themes.default.renderers.Label"]

function Label:__init() super()
end

function Label:render(context, label, style)
	if label.textChanged then
		label.renderedText = label.font:render(label.text, label.fontSize)		
		label.textChanged = false
	end
	context:setColor(label:color())
	--context:drawRectFilled(label:globalRect())
	gl.glMatrixMode(gl.GL_MODELVIEW)
	gl.glLoadIdentity()
	local gr = label:globalRect()
	gl.glTranslate(gr.x, gr.y, 0)
	label.renderedText:render(context)
	gl.glLoadIdentity()
end
