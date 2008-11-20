guiro:Renderer
{
  render = function(self, context, slider, style)
    local globalRect = slider:globalRect()
    
    context:setColor(style.color)
    context:drawRectFilled(globalRect)
    context:setColor(style.lineColor)
    context:drawRectOutline(globalRect)
    if (slider.orientation == lost.guiro.controls.Slider.Orientation.horizontal) then
      context:drawLine(lost.math.Vec2(globalRect.x, globalRect.y + globalRect.height / 2),
                       lost.math.Vec2(globalRect.x + globalRect.width, globalRect.y + globalRect.height / 2))
    elseif (slider.orientation == lost.guiro.controls.Slider.Orientation.vertical) then
      context:drawLine(lost.math.Vec2(globalRect.x + globalRect.width / 2, globalRect.y),
                       lost.math.Vec2(globalRect.x + globalRect.width / 2, globalRect.y + globalRect.height))
    end

    slider.style.button.renderer:render(context, slider.button, slider.style.button.style)
  end
}
