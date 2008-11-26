module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Renderer
  ]]

require("lost.io.Loader")
require("lost.guiro.themes.Renderer")

function Loader:RendererFromFile(filename)
  local result
  local guiro = self
  guiro.Renderer = function(self, definition) result = self:applyDefinitionToRenderer(lost.guiro.themes.Renderer(), definition) end
  self:executeScript(filename, {guiro = guiro, lost = lost, math = math, log = log, gl = gl, tostring = tostring, os = os, print = print})
  return result
end

function Loader:applyDefinitionToRenderer(renderer, definition)
  -- apply properties
  if (renderer and definition) then
    for name,value in next,definition do
      if type(name) == "string" then
        renderer[name] = value
      end
    end
  end
  return renderer
end
