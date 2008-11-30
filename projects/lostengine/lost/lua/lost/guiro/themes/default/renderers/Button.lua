module("lost.guiro.themes.default.renderers", package.seeall)

--[[
     Button Renderer
  ]]
require("lost.guiro.Renderer")

class "lost.guiro.themes.default.renderers.Button" (lost.guiro.Renderer)
Button = _G["lost.guiro.themes.default.renderers.Button"]

function Button:__init() super()
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

  if button.images and button.images[button.state] then
    if not button.images[button.state].parent then
      button:appendChild(button.images[button.state])
    end
    button.images[button.state].color = self.color
    button.images[button.state]:render(context)
  else
    context:setColor(self.color)
    context:drawRectFilled(globalRect)
  end

  if button.label then
    if not button.label.parent then
      button:appendChild(button.label)
    end
    button.label.color = self.fontColor
    button.label:render(context)
  end

  context:setColor(self.lineColor)
  context:drawRectOutline(globalRect)
end
