guiro:Renderer
{
  fade = function(self, newColor)
    if self.color.r < newColor.r then
      self.color.r = self.color.r + 0.05
    elseif self.color.r > newColor.r then
      self.color.r = self.color.r - 0.05
    end
    if self.color.g < newColor.g then
      self.color.g = self.color.g + 0.05
    elseif self.color.g > newColor.g then
      self.color.g = self.color.g - 0.05
    end
    if self.color.b < newColor.b then
      self.color.b = self.color.b + 0.05
    elseif self.color.b > newColor.b then
      self.color.b = self.color.b - 0.05
    end
    if self.color.a < newColor.a then
      self.color.a = self.color.a + 0.05
    elseif self.color.a > newColor.a then
      self.color.a = self.color.a - 0.05
    end
  end,

  render = function(self, context, button, style)
    local globalRect = button:globalRect()

    if button.fadeStates then
      if not self.color then
        self.color = lost.common.Color(style[button.state].color)
      end
      self:fade(style[button.state].color)
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
