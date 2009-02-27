module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Font
  ]]

require("lost.io.Loader")

function Loader:Font(filename)
	if not self.freetypeLibrary then
		self.freetypeLibrary = lost.font.freetype.Library.create()
	end
	return lost.font.TrueTypeFont.create(self.freetypeLibrary, self.loader:load(filename))
end
