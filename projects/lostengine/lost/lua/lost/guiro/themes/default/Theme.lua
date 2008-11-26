module("lost.guiro.themes", package.seeall)

--[[
     Theme definition
  ]]

AnimatedImage =
{
  Renderer = guiro:RendererFromFile("lost/guiro/themes/default/renderer/AnimatedImage.lua"),
  Style = guiro:StyleFromFile("lost/guiro/themes/default/styles/AnimatedImage.lua")
}

Button =
{
  Renderer = guiro:RendererFromFile("lost/guiro/themes/default/renderer/Button.lua"),
  Style = guiro:StyleFromFile("lost/guiro/themes/default/styles/Button.lua")
}

Image =
{
  Renderer = guiro:RendererFromFile("lost/guiro/themes/default/renderer/Image.lua"),
  Style = guiro:StyleFromFile("lost/guiro/themes/default/styles/Image.lua")
}

Label =
{
  Renderer = guiro:RendererFromFile("lost/guiro/themes/default/renderer/Label.lua"),
  Style = guiro:StyleFromFile("lost/guiro/themes/default/styles/Label.lua")
}

Slider =
{
  Renderer = guiro:RendererFromFile("lost/guiro/themes/default/renderer/Slider.lua"),
  Style = guiro:StyleFromFile("lost/guiro/themes/default/styles/Slider.lua")
}

Window =
{
  Renderer = guiro:RendererFromFile("lost/guiro/themes/default/renderer/Window.lua"),
  Style = guiro:StyleFromFile("lost/guiro/themes/default/styles/Window.lua")
}
