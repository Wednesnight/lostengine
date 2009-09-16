-- lost.guiro.themes.default.styles.Window
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.Window" "lost.guiro.Style" {}

function Window:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.Window"
end

function Window:apply(target)
	local c = .27843
	local windowGray = Color(c,c,c,1);
  target:showFrame(true)
  target:showBackground(true)
  target:backgroundColor(windowGray)
  target:frameColor(Color(1,1,1,1))
end
