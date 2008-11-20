module("lost.guiro.renderer", package.seeall)

--[[
     Renderer
  ]]
class "lost.guiro.renderer.Renderer"
Renderer = _G["lost.guiro.renderer.Renderer"]

function Renderer:fade(currentColor, targetColor)
  if currentColor.r < newColor.r then
    currentColor.r = currentColor.r + 0.05
  elseif currentColor.r > newColor.r then
    currentColor.r = currentColor.r - 0.05
  end
  if currentColor.g < newColor.g then
    currentColor.g = currentColor.g + 0.05
  elseif currentColor.g > newColor.g then
    currentColor.g = currentColor.g - 0.05
  end
  if currentColor.b < newColor.b then
    currentColor.b = currentColor.b + 0.05
  elseif currentColor.b > newColor.b then
    currentColor.b = currentColor.b - 0.05
  end
  if currentColor.a < newColor.a then
    currentColor.a = currentColor.a + 0.05
  elseif currentColor.a > newColor.a then
    currentColor.a = currentColor.a - 0.05
  end
end

function Renderer:render(context, object, style)
  -- empty
end
