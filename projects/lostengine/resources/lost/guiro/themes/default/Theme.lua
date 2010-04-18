-- lost.guiro.themes.default.Theme
module("lost.guiro.themes.default", package.seeall) 

require("lost.common.Class")
require("lost.guiro.Theme")

require("lost.guiro.themes.default.styles.Image")
require("lost.guiro.themes.default.styles.Label")
require("lost.guiro.themes.default.styles.Screen")
require("lost.guiro.themes.default.styles.UserInterface")
require("lost.guiro.themes.default.styles.View")
require("lost.guiro.themes.default.styles.VBox")
require("lost.guiro.themes.default.styles.HBox")
require("lost.guiro.themes.default.styles.Window")
require("lost.guiro.themes.default.styles.Button")
require("lost.guiro.themes.default.styles.RenderView")
require("lost.guiro.themes.default.styles.SpinEdit")

using("lost.math.Vec2")

lost.common.Class "lost.guiro.themes.default.Theme" "lost.guiro.Theme" {}

function Theme:constructor(loader, textureManager)
  lost.guiro.Theme(self, loader)
  self.ftlib = lost.font.freetype.Library.create()
  self.defaultFont = lost.font.TrueTypeFont.create(self.ftlib, loader:load("lost/resources/ttf-bitstream-vera-1.10/Vera.ttf"))
  self.defaultFont.atlasSize = Vec2(256, 256)
  self.name = "default"
  self.styles = {} -- FIXME why is styles not properly initialised from base class constructor?
  self:addStyle(lost.guiro.themes.default.styles.View())
  self:addStyle(lost.guiro.themes.default.styles.VBox())
  self:addStyle(lost.guiro.themes.default.styles.HBox())
  self:addStyle(lost.guiro.themes.default.styles.Screen())
  self:addStyle(lost.guiro.themes.default.styles.Window())
  self:addStyle(lost.guiro.themes.default.styles.Button(loader, self.defaultFont, textureManager))
  self:addStyle(lost.guiro.themes.default.styles.Image())
  self:addStyle(lost.guiro.themes.default.styles.UserInterface())
  self:addStyle(lost.guiro.themes.default.styles.Label(self.defaultFont))
  self:addStyle(lost.guiro.themes.default.styles.RenderView())
  self:addStyle(lost.guiro.themes.default.styles.SpinEdit(textureManager))
end
