module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Renderer
  ]]

require("lost.io.Loader")
require("lost.guiro.themes.Renderer")

function Loader:Renderer(definition)
  return self:applyDefinitionToRenderer(lost.guiro.themes.Renderer(), definition)
end

function Loader:applyDefinitionToRenderer(renderer, definition)
  if (renderer and definition) then
    for key,value in next,definition do
      renderer[key] = value
    end
  end
  return renderer
end
