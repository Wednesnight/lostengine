module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.common.Object")

--[[
     Renderer
  ]]
Renderer = lost.common.Class("lost.guiro.Renderer", lost.common.Object)

function Renderer:__init(properties) lost.common.Object.__init(self)
  properties = properties or {}
end

--[[
    render
  ]]
function Renderer:render(canvas, view, style)
  local globalRect = view:globalRect()

  if style.backgroundColor then
    canvas:setColor(style.backgroundColor)
    if style.corners.size then
      if style.corners.steps then
        canvas:drawRectFilledRounded(globalRect, style.corners.size, style.corners.steps)
      else
        canvas:drawRectFilledRounded(globalRect, style.corners.size, 25)
      end
    else
      canvas:drawRectFilled(globalRect)
    end
  end

  if style.borderColor then
    canvas:setColor(style.borderColor)
    if style.corners.size then
      if style.corners.steps then
        canvas:drawRectOutlineRounded(globalRect, style.corners.size, style.corners.steps)
      else
        canvas:drawRectOutlineRounded(globalRect, style.corners.size, 25)
      end
    else
      canvas:drawRectOutline(globalRect)
    end
  end
end

function Renderer:fade(currentColor, targetColor)
  if (currentColor.r - targetColor.r) > -0.05 and
     (currentColor.r - targetColor.r) < 0.05 then
    currentColor.r = targetColor.r
  elseif currentColor.r < targetColor.r then
    currentColor.r = currentColor.r + 0.05
  elseif currentColor.r > targetColor.r then
    currentColor.r = currentColor.r - 0.05
  end
  if (currentColor.g - targetColor.g) > -0.05 and
     (currentColor.g - targetColor.g) < 0.05 then
    currentColor.g = targetColor.g
  elseif currentColor.g < targetColor.g then
    currentColor.g = currentColor.g + 0.05
  elseif currentColor.g > targetColor.g then
    currentColor.g = currentColor.g - 0.05
  end
  if (currentColor.b - targetColor.b) > -0.05 and
     (currentColor.b - targetColor.b) < 0.05 then
    currentColor.b = targetColor.b
  elseif currentColor.b < targetColor.b then
    currentColor.b = currentColor.b + 0.05
  elseif currentColor.b > targetColor.b then
    currentColor.b = currentColor.b - 0.05
  end
  if (currentColor.a - targetColor.a) > -0.05 and
     (currentColor.a - targetColor.a) < 0.05 then
    currentColor.a = targetColor.a
  elseif currentColor.a < targetColor.a then
    currentColor.a = currentColor.a + 0.05
  elseif currentColor.a > targetColor.a then
    currentColor.a = currentColor.a - 0.05
  end
end
