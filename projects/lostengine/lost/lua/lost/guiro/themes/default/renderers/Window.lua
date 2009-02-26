module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Window Renderer
  ]]
Window = lost.common.Class("lost.guiro.themes.default.renderers.Window", lost.guiro.Renderer)

function Window:__init() lost.guiro.Renderer.__init(self)
end

function Window:render(canvas, window, style)
  local globalRect = window:globalRect()

  canvas:setColor(style.color)
  canvas:drawRectFilled(globalRect)
  canvas:setColor(style.header.color)
  canvas:drawRectFilled(lost.math.Rect(globalRect.x, 
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
    window.label:render(canvas)
  end

  canvas:setColor(style.lineColor)
  canvas:drawRectOutline(globalRect)
end
