-- lost.guiro.themes.default.styles.View
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.View" "lost.guiro.Style" {}

function View:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.View"
end

function View:apply(target)
end