module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.UserInterface" "lost.guiro.view.View" {}

function UserInterface:constructor(textureManager)
  lost.guiro.view.View.constructor(self, textureManager)
end
