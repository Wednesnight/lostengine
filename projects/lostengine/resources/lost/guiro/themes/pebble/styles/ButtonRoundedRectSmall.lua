-- lost.guiro.themes.pebble.styles.ButtonRoundedRectSmall
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.guiro.themes.pebble.styles.ButtonRoundedRect")

lost.common.Class "lost.guiro.themes.pebble.styles.ButtonRoundedRectSmall" "lost.guiro.themes.pebble.styles.ButtonRoundedRect" {}

function ButtonRoundedRectSmall:constructor(loader, df, textureManager)
  lost.guiro.themes.pebble.styles.ButtonRoundedRect.constructor(self, loader, df, textureManager)
  self.name = "RoundedRectSmall"
  self.fontSize = 10
  self.buttonHeight = 16
	self.cornerRadius = self.buttonHeight / 2  
end