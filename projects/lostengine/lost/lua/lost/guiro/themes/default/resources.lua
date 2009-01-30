module("lost.guiro.themes.default", package.seeall)

--[[
  Resources
]]

-- loader
loader = environment.application.loader

-- fonts
freetypeLibrary = lost.font.freetype.Library.create()
defaultFont = lost.font.TrueTypeFont.create(freetypeLibrary, loader:load("Vera.ttf"))
