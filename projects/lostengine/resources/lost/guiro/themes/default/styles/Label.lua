-- lost.guiro.themes.default.styles.Label
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color
local Vec2 = lost.math.Vec2

lost.common.Class "lost.guiro.themes.default.styles.Label" "lost.guiro.Style" {}

function Label:constructor(fnt)
  lost.guiro.Style(self)
  self.fnt = fnt
  self.name = "default"
  self.targetClassName = "lost.guiro.Label"
end

function Label:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  target:font(self.fnt)
  target:fontSize(12)
  target:text("Label")
  target:textColor(Color(1,1,1,1))
  target:halign("center")
  target:valign("center")
  target:showShadow(true)
  target:shadowColor(Color(0,0,0,1))
  target:shadowOffset(Vec2(1,-1))
end
