-- lost.guiro.themes.pebble.styles.View
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

using("lost.common.Color")

lost.common.Class "lost.guiro.themes.pebble.styles.ViewDefault" "lost.guiro.Style" {}

function ViewDefault:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.view.View"
end

function ViewDefault:apply(target)
end
