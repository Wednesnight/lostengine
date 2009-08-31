module("lost.guiro", package.seeall)

require("lost.guiro.Label")
require("lost.guiro.Image")

lost.common.Class "lost.guiro.Button" "lost.guiro.View" {}

function Button:constructor()
	lost.guiro.View.constructor(self)
	
end


