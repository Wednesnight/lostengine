-- lost.guiro.themes.default.styles.Button
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.Button" "lost.guiro.Style" {}

function Button:constructor(loader)
  lost.guiro.Style(self)
  self.normalBitmap = loader:load("lost/guiro/themes/default/resources/ButtonNormal.png")
  self.hoverBitmap = loader:load("lost/guiro/themes/default/resources/ButtonHover.png")
  self.pushedBitmap = loader:load("lost/guiro/themes/default/resources/ButtonPushed.png")
  self.disabledBitmap = loader:load("lost/guiro/themes/default/resources/ButtonDisabled.png")
  self.name = "default"
  self.targetClassName = "lost.guiro.Button"
end

function Button:apply(target)
  target:showFrame(true)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
end
