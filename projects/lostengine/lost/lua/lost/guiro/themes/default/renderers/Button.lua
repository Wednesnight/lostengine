module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Button Renderer
  ]]
Button = lost.common.Class("lost.guiro.themes.default.renderers.Button", lost.guiro.Renderer)

function Button:__init() lost.guiro.Renderer.__init(self)
end

function Button:render(context, button, style)
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

  if self.color ~= style[button.state].color or
     self.fontColor ~= style[button.state].fontColor or
     self.lineColor ~= style[button.state].lineColor
  then
    button:needsRedraw()
  end

  if self.currentImage then
    if not button.images or not rawequal(self.currentImage, button.images[button.state]) then
      button:removeChild(self.currentImage)
      self.currentImage = nil
    end
  end
  if button.images and button.images[button.state] then
    if not rawequal(self.currentImage, button.images[button.state]) then
      self.currentImage = button.images[button.state]
    end
    if not self.currentImage.parent then
      button:appendChild(self.currentImage, 1)
      self.currentImage:needsLayout()
      self.currentImage:needsRedraw()
    end
    self.currentImage:color(self.color)
  else
    context:setColor(self.color)
    context:drawRectFilled(globalRect)
  end

  if button.label then
    if not button.label.parent then
      button:appendChild(button.label)
    end
    if button.label:color() ~= self.fontColor then
      button.label:color(self.fontColor)
      button.label:needsRedraw()
    end
    button.label:render(context)
  end

  context:setColor(self.lineColor)
  context:drawRectOutline(globalRect)
end
