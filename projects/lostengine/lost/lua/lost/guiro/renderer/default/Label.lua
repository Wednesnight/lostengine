guiro:Renderer
{
	render = function(self, context, label, style)
		context:setColor(label.color)
		--context:drawRectFilled(label:globalRect())
		gl.glMatrixMode(gl.GL_MODELVIEW)
		gl.glLoadIdentity()
		gr = label:globalRect()
		gl.glTranslate(gr.x, gr.y, 0)
		label.renderedText:render(context)
		gl.glLoadIdentity()
	end
}