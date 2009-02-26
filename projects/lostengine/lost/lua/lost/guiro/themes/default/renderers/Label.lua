module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Label Renderer
  ]]
Label = lost.common.Class("lost.guiro.themes.default.renderers.Label", lost.guiro.Renderer)

function Label:__init() lost.guiro.Renderer.__init(self)
end

function Label:render(canvas, label, style)
	if label.textChanged then
		label.renderedText = label.font:render(label.text, label.fontSize)		
		label.textChanged = false
	end
	canvas:setColor(label:color())
--	canvas:drawRectFilled(label:globalRect())
	gl.glPushMatrix()
	local gr = label:globalRect()
	gl.glTranslate(gr.x, gr.y, 0)
	label.renderedText:render(canvas)
	gl.glPopMatrix()
end
