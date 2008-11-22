guiro:Renderer
{
	render = function(self, context, label, style)
		context:setColor(label.color)
		context:drawRectFilled(label:globalRect())
	end
}