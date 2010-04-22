-- lost.guiro.themes.pebble.Theme
module("lost.guiro.themes.pebble", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.themes.pebble.styles.ViewDefault")
require("lost.guiro.themes.pebble.styles.ButtonRoundedRect")

using("lost.math.Vec2")

lost.common.Class "lost.guiro.themes.pebble.Theme" "lost.guiro.Theme" {}

function Theme:constructor(loader, textureManager)
  lost.guiro.Theme(self, loader)
  self.textureManager = textureManager
  self.ftlib = lost.font.freetype.Library.create()
  self.defaultFont = lost.font.TrueTypeFont.create(self.ftlib, loader:load("lost/resources/ttf-bitstream-vera-1.10/Vera.ttf"))
  self.defaultFont.atlasSize = Vec2(256, 256)
  self.name = "pebble"
  self.styles = {} -- FIXME why is styles not properly initialised from base class constructor?  
  self:addStyle(lost.guiro.themes.pebble.styles.ViewDefault())  
  self:addStyle(lost.guiro.themes.pebble.styles.ButtonRoundedRect(loader, self.defaultFont, textureManager))  
end