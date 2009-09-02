module("lost.guiro", package.seeall)

require("lost.guiro.Label")
require("lost.guiro.Image")

lost.common.Class "lost.guiro.Button" "lost.guiro.View" {}

Button.STATE_NORMAL = "normal"
Button.STATE_HOVER = "hover"
Button.STATE_PUSHED = "pushed"
Button.STATE_DISABLED = "disabled"

function Button:constructor()
	lost.guiro.View.constructor(self)
	self._state = Button.STATE_NORMAL
end


