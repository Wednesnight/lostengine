-- lost.guiro.themes.pebble.styles.ButtonRoundedRectRegular
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.guiro.themes.pebble.styles.ButtonRoundedRect")

lost.common.Class "lost.guiro.themes.pebble.styles.ButtonRoundedRectRegular" "lost.guiro.themes.pebble.styles.ButtonRoundedRect" {}

function ButtonRoundedRectRegular:constructor(loader, df)
  lost.guiro.themes.pebble.styles.ButtonRoundedRect.constructor(self, loader, df)
  self.name = "RoundedRectRegular"
  self.fontSize = 11
  self.buttonHeight = 18
	self.cornerRadius = self.buttonHeight / 2  
end