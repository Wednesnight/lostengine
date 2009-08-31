module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.HBox" "lost.guiro.View" {}

function HBox:constructor()
	lost.guiro.View.constructor(self)
	
end


