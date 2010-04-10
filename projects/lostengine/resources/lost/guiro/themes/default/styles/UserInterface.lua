-- lost.guiro.themes.default.styles.UserInterface
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.UserInterface" "lost.guiro.Style" {}

function UserInterface:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.UserInterface"
end

function UserInterface:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
end