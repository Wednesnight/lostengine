module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Label Renderer
  ]]
Label = lost.common.Class("lost.guiro.themes.default.renderers.Label", lost.guiro.Renderer)

function Label:__init() lost.guiro.Renderer.__init(self)
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
