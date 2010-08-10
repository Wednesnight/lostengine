-- lost.guiro.themes.default.styles.Image
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.Image" "lost.guiro.Style" {}

function Image:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.view.Image"
end

function Image:apply(target)
  target:showFrame(true)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  target:scale("aspect")
end
