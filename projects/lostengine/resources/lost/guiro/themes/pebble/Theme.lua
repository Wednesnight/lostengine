-- lost.guiro.themes.pebble.Theme
module("lost.guiro.themes.pebble", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.themes.pebble.styles.ViewDefault")
require("lost.guiro.themes.pebble.styles.ButtonRoundedRect")
require("lost.guiro.themes.pebble.styles.ButtonRoundedRectRegular")
require("lost.guiro.themes.pebble.styles.ButtonRoundedRectSmall")
require("lost.guiro.themes.pebble.styles.ButtonRoundedRectMini")
require("lost.guiro.themes.pebble.buildGradients")
require("lost.guiro.themes.pebble.styles.ViewBubbleGrey")

using("lost.math.Vec2")
using("lost.guiro.themes.pebble.buildGradients")

lost.common.Class "lost.guiro.themes.pebble.Theme" "lost.guiro.Theme" {}

function Theme:constructor(loader, textureManager)
  lost.guiro.Theme(self, loader)
  buildGradients(textureManager);
  self.textureManager = textureManager
  self.ftlib = lost.font.freetype.Library.create()
  self.defaultFont = lost.font.TrueTypeFont.create(self.ftlib, loader:load("lost/resources/ttf-bitstream-vera-1.10/Vera.ttf"))
  self.defaultFont.atlasSize = Vec2(256, 256)
  self.name = "pebble"
  self.styles = {} -- FIXME why is styles not properly initialised from base class constructor?  
  self:addStyle(lost.guiro.themes.pebble.styles.ViewDefault())  
--  self:addStyle(lost.guiro.themes.pebble.styles.ButtonRoundedRect(loader, self.defaultFont, textureManager))  
  self:addStyle(lost.guiro.themes.pebble.styles.ButtonRoundedRectRegular(loader, self.defaultFont, textureManager))  
  self:addStyle(lost.guiro.themes.pebble.styles.ButtonRoundedRectSmall(loader, self.defaultFont, textureManager))  
  self:addStyle(lost.guiro.themes.pebble.styles.ButtonRoundedRectMini(loader, self.defaultFont, textureManager))  
  self:addStyle(lost.guiro.themes.pebble.styles.ViewBubbleGrey())
end