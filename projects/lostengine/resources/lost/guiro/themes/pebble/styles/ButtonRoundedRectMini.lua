-- lost.guiro.themes.pebble.styles.ButtonRoundedRectMini
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.guiro.themes.pebble.styles.ButtonRoundedRect")

lost.common.Class "lost.guiro.themes.pebble.styles.ButtonRoundedRectMini" "lost.guiro.themes.pebble.styles.ButtonRoundedRect" {}

function ButtonRoundedRectMini:constructor(loader, df)
  lost.guiro.themes.pebble.styles.ButtonRoundedRect.constructor(self, loader, df)
  self.name = "RoundedRectMini"
  self.fontSize = 8
  self.buttonHeight = 14
	self.cornerRadius = self.buttonHeight / 2  
end