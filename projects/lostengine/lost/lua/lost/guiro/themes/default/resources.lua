module("lost.guiro.themes.default", package.seeall)

--[[
  Resources
]]

-- loader
loader = lost.resource.currentLoader

-- fonts
freetypeLibrary = lost.font.freetype.Library.create()
defaultFont = lost.font.TrueTypeFont.create(freetypeLibrary, loader:load("Vera.ttf"))
