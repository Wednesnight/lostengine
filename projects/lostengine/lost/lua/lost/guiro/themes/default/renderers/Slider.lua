module("lost.guiro.themes.default.renderers", package.seeall)

require("lost.guiro.Renderer")

--[[
     Slider Renderer
  ]]
Slider = lost.common.Class("lost.guiro.themes.default.renderers.Slider", lost.guiro.Renderer)

function Slider:__init(properties) lost.guiro.Renderer.__init(self, properties)
  properties = properties or {}
end

function Slider:render(canvas, slider, style)
  local globalRect = slider:globalRect()
  
  canvas:setColor(style.color)
  canvas:drawRectFilled(globalRect)
  canvas:setColor(style.lineColor)
  canvas:drawRectOutline(globalRect)
  if (slider.orientation == lost.guiro.controls.Slider.Orientation.horizontal) then
    canvas:drawLine(lost.math.Vec2(globalRect.x, globalRect:maxY() - globalRect.height / 2),
                     lost.math.Vec2(globalRect:maxX(), globalRect:maxY() - globalRect.height / 2))
  elseif (slider.orientation == lost.guiro.controls.Slider.Orientation.vertical) then
    canvas:drawLine(lost.math.Vec2(globalRect:maxX() - globalRect.width / 2, globalRect.y),
                     lost.math.Vec2(globalRect:maxX() - globalRect.width / 2, globalRect:maxY()))
  end

  slider.button:render(canvas)
end
