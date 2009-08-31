module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.VBox" "lost.guiro.View" {}

function VBox:constructor()
	lost.guiro.View.constructor(self)
end


