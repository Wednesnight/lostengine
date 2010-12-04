module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.Label" "lost.guiro.view.View" {}

function Label:constructor(args)
  lost.guiro.view.View.constructor(self, args)
end



