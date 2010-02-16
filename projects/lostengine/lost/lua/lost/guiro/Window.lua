module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.Window" "lost.guiro.View" {}

function Window:constructor()
  lost.guiro.View.constructor(self)
end
