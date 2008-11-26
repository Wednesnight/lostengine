module("lost.guiro.themes.default.renderer", package.seeall)

--[[
     Window Renderer
  ]]
require("lost.guiro.themes.Renderer")

class "lost.guiro.themes.default.renderer.Window" (lost.guiro.themes.Renderer)
Window = _G["lost.guiro.themes.default.renderer.Window"]

function Window:__init() super()
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
      window.label:setParent(window)
    end
    window.label.color = style.header.fontColor
    window.label:render(context)
  end

  context:setColor(style.lineColor)
  context:drawRectOutline(globalRect)
end
