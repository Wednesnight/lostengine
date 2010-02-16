-- lost.guiro.themes.default.styles.VBox
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.VBox" "lost.guiro.Style" {}

function VBox:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.VBox"
end

function VBox:apply(target)
end
