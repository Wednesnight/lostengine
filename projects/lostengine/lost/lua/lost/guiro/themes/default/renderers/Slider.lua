module("lost.guiro.themes.default.renderers", package.seeall)

--[[
     Slider Renderer
  ]]
require("lost.guiro.Renderer")

class "lost.guiro.themes.default.renderers.Slider" (lost.guiro.Renderer)
Slider = _G["lost.guiro.themes.default.renderers.Slider"]

function Slider:__init() super()
end

function Slider:render(context, slider, style)
  local globalRect = slider:globalRect()
  
  context:setColor(style.color)
  context:drawRectFilled(globalRect)
  context:setColor(style.lineColor)
  context:drawRectOutline(globalRect)
  if (slider.orientation == lost.guiro.controls.Slider.Orientation.horizontal) then
    context:drawLine(lost.math.Vec2(globalRect.x, globalRect.y + globalRect.height / 2),
                     lost.math.Vec2(globalRect:maxX(), globalRect.y + globalRect.height / 2))
  elseif (slider.orientation == lost.guiro.controls.Slider.Orientation.vertical) then
    context:drawLine(lost.math.Vec2(globalRect.x + globalRect.width / 2, globalRect.y),
                     lost.math.Vec2(globalRect.x + globalRect.width / 2, globalRect:maxY()))
  end

  slider.button:render(context)
end
