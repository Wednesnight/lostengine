-- lost.guiro.themes.pebble.styles.ViewBubbleGrey
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

using("lost.common.Color")

lost.common.Class "lost.guiro.themes.pebble.styles.ViewBubbleGrey" "lost.guiro.Style" {}

function ViewBubbleGrey:constructor()
  lost.guiro.Style(self)
  self.name = "bubbleGrey"
  self.targetClassName = "lost.guiro.View"
  self.sizes = {}
  self.sizes["regular"] = 16;
  self.sizes["small"] = 12;
  self.sizes["mini"] = 10;
end

function ViewBubbleGrey:apply(target, def)
  local size = 16
  if def and def.size then
    size = self.sizes[def.size] or size
  end
  local radius = size / 2
  target.bounds.width = lost.guiro.wsize{abs=size}
  target.bounds.height = lost.guiro.hsize{abs=size}
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
