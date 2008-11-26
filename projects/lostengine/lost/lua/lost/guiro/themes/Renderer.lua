module("lost.guiro.themes", package.seeall)

--[[
     Renderer
  ]]
class "lost.guiro.themes.Renderer"
Renderer = _G["lost.guiro.themes.Renderer"]

function Renderer:__init()
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

function Renderer:render(context, object, style)
  -- empty
end
