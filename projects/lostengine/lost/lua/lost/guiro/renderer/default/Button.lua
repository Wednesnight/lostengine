guiro:Renderer
{
  render = function(self, context, button, style)
    local globalRect = button:globalRect()

    if button.fadeStates then
      if not self.color then
        self.color = lost.common.Color(style[button.state].color)
      end
      self:fade(self.color, style[button.state].color)
      if not self.fontColor then
        self.fontColor = lost.common.Color(style[button.state].fontColor)
      end
      self:fade(self.fontColor, style[button.state].fontColor)
      if not self.lineColor then
        self.lineColor = lost.common.Color(style[button.state].lineColor)
      end
      self:fade(self.lineColor, style[button.state].lineColor)
    else
      self.color = lost.common.Color(style[button.state].color)
      self.fontColor = lost.common.Color(style[button.state].fontColor)
      self.lineColor = lost.common.Color(style[button.state].lineColor)
    end

    context:setColor(self.color)
    if button.images and button.images[button.state] then
      if not button.images[button.state].parent then
        button.images[button.state]:setParent(button)
      end
      button.images[button.state].color = self.color
      button.images[button.state].renderer:render(context, button.images[button.state], button.images[button.state].style)
    else
      context:drawRectFilled(globalRect)
    end

    if button.label then
      button.label.color = self.fontColor
      button.label.renderer:render(context, button.label, button.label.style)
    end

    context:setColor(self.lineColor)
    context:drawRectOutline(globalRect)
  end
}
