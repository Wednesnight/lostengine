module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.Window" "lost.guiro.view.View" {}

function Window:constructor(textureManager)
  lost.guiro.view.View.constructor(self, textureManager)
end
