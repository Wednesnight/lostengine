module("lost.guiro", package.seeall) -- TextureManager

require("lost.common.Class")

lost.common.Class "lost.guiro.TextureManager" {}

function TextureManager:constructor(gradientMaxWidthPixels)
  self._textureManager = lost.mesh.TextureManager.create()
  self._gradientName2Coord = {}
  self._textureManager.gradientTextureWidth = gradientMaxWidthPixels
end

function TextureManager:addGradient(name, colorGradient)
  local coord = self._textureManager:addGradient(colorGradient)
  if self._gradientName2Coord[name] then
    log.warn("overwriting gradient "..tostring(name))
  end
  self._gradientName2Coord[name] = coord
end

function TextureManager:hasGradient(name)
  return (self._gradientName2Coord[name] ~= nil)
end

function TextureManager:gradientCoord(name)
  local result = self._gradientName2Coord[name]
  if not result then
    log.warn("couldn't find gradient with name "..tostring(name))
    result = 0
  end
  return result
end