module("lost.guiro", package.seeall)

config = {}

-- loader
config.loader = loader

-- font lib
config.freetypeLibrary = lost.font.freetype.Library.create()

-- default font
config.defaultFont = lost.font.TrueTypeFont.create(config.freetypeLibrary, config.loader:load("Vera.ttf"))
