module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Font
  ]]

require("lost.io.Loader")

function Loader:Font(filename)
	if not self.freetypeLibrary then
		self.freetypeLibrary = lost.font.freetype.Library.create()
	end
	local file = self.loader:load(filename)
	local newfont = lost.font.TrueTypeFont.create(self.freetypeLibrary, file)
	return newfont
end
