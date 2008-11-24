guiro:Renderer
{
  render = function(self, context, window, style)
    local globalRect = window:globalRect()

    context:setColor(style.color)
    context:drawRectFilled(globalRect)
    context:setColor(style.header.color)
    context:drawRectFilled(lost.math.Rect(globalRect.x, 
                                          globalRect.y + (globalRect.height - window.header.height),
                                          globalRect.width,
                                          window.header.height))

    if window.label then
      if not window.label.parent then
        window.label:setParent(window)
      end
      window.label.color = style.header.fontColor
      window.label.renderer:render(context, window.label, window.label.style)
    end

    context:setColor(style.lineColor)
    context:drawRectOutline(globalRect)
  end
}
