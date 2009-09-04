module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.Window" "lost.guiro.View" {}

function Window:constructor()
  lost.guiro.View.constructor(self)
  self.doScissor = true
end
