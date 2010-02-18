-- lost.guiro.themes.default.styles.RenderView
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.themes.default.styles.Image")

lost.common.Class "lost.guiro.themes.default.styles.RenderView" "lost.guiro.themes.default.styles.Image" {}

function RenderView:constructor()
  lost.guiro.themes.default.styles.Image.constructor(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.RenderView"
end

function RenderView:apply(target)
  lost.guiro.themes.default.styles.Image.apply(self, target)
end
