module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Font
  ]]

require("lost.io.Loader")

function Loader:Font(filename)
  	if not self.freetypeLibrary then
		self.freetypeLibrary = lost.font.freetype.Library.create()
	end
	file = self.loader:load(filename)
	newfont = lost.font.TrueTypeFont.create(self.freetypeLibrary, file)
	return newfont
end
