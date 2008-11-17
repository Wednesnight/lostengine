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

    if not self.color then
      self.color = lost.common.Color(style[button.state].color)
    end
    self:fade(style[button.state].color)

    context:setColor(self.color)
    context:drawRectFilled(globalRect)
    context:setColor(style[button.state].lineColor)
    context:drawRectOutline(globalRect)
  end
}
