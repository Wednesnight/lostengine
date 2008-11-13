guiro:Renderer
{
  render = function(self, context, button, style)
    local globalRect = button:globalRect()

    context:setColor(style[button.state].color)
    context:drawRectFilled(globalRect)
    context:setColor(style[button.state].lineColor)
    context:drawRectOutline(globalRect)
  end
}
