-- lost.guiro.themes.pebble.styles.View
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

using("lost.common.Color")

lost.common.Class "lost.guiro.themes.pebble.styles.ViewBubbleGrey" "lost.guiro.Style" {}

function ViewBubbleGrey:constructor()
  lost.guiro.Style(self)
  self.name = "bubbleGrey"
  self.targetClassName = "lost.guiro.View"
end

function ViewBubbleGrey:apply(target)
  local size = 16
  local radius = size / 2
  target.bounds.width = lost.guiro.wabs(size)
  target.bounds.height = lost.guiro.habs(size)
  target:backgroundCornerRadius(radius)
  target:frameCornerRadius(radius)
  target:frameRoundCorners(true, true, true, true)
  target:backgroundRoundCorners(true, true, true, true)
  target:showFrame(true)
  target:showBackground(true)
  target:backgroundColor(Color(1,1,1))
  target:frameColor(Color(1,1,1))
  target:backgroundGradient("bubbleGreyBg")
  target:frameGradient("bubbleGreyFrame")
end
