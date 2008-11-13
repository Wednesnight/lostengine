guiro:Renderer
{
  render = function(self, context, window, style)
    local globalRect = window:globalRect()

    context:setColor(style.color)
    context:drawRectFilled(globalRect)
    context:setColor(style.header.color)
    context:drawRectFilled(lost.math.Rect(globalRect.x, 
                                          globalRect.y + (globalRect.height - style.header.height),
                                          globalRect.width,
                                          style.header.height))
    context:setColor(style.lineColor)
    context:drawRectOutline(globalRect)
  end
}
