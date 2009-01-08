module("lost.guiro.themes.default.renderers", package.seeall)

--[[
     Window Renderer
  ]]
require("lost.guiro.Renderer")

class "lost.guiro.themes.default.renderers.Window" (lost.guiro.Renderer)
Window = _G["lost.guiro.themes.default.renderers.Window"]

function Window:__init() lost.guiro.Renderer.__init(self)
end

function Window:render(context, window, style)
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
      window:appendChild(window.label)
    end
    if window.label:color() ~= style.header.fontColor then
      window.label:color(style.header.fontColor)
      window.label:needsRedraw()
    end
    window.label:render(context)
  end

  context:setColor(style.lineColor)
  context:drawRectOutline(globalRect)
end
