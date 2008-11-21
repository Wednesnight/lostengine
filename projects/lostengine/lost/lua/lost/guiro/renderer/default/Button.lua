guiro:Renderer
{
  render = function(self, context, button, style)
    local globalRect = button:globalRect()

    if button.fadeStates then
      if not self.color then
        self.color = lost.common.Color(style[button.state].color)
      end
      self:fade(self.color, style[button.state].color)
      context:setColor(self.color)
    else
      context:setColor(style[button.state].color)
    end

    if button.images and button.images[button.state] then
      if not button.images[button.state].parent then
        button.images[button.state]:setParent(button)
      end
      button.images[button.state].color = self.color
      button.images[button.state].renderer:render(context, button.images[button.state], button.images[button.state].style)
    else
      context:drawRectFilled(globalRect)
    end
    context:setColor(style[button.state].lineColor)
    context:drawRectOutline(globalRect)
  end
}
