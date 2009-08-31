module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.VBox" "lost.guiro.View" {}

function VBox:constructor()
	lost.guiro.View.constructor(self)
	self.mode = "spread" -- spread or stack
	self.valign = "top" -- only used if mode != spread
	self.halign = "center"
	self.spacing = 0 -- only used when mode is stacked
end

function VBox:recalculateSubviewPositions()
--	log.debug("start ---")
	local numViews = #(self.subviews)
	local relfactor = 1/(numViews-1)
	local yoffset = 0
--	log.debug("relfactor "..relfactor)
	local f = 1
	for k,view in pairs(self.subviews) do
		log.debug("f "..f)
		view.bounds.y = lost.guiro.ycenter{rel=f}		
		f = f- relfactor
		view.bounds.x = lost.guiro.xcenter()
	end
--	log.debug("end ---")
end

function VBox:addSubview(newview, pos)
	lost.guiro.View.addSubview(self, newview, pos)
	self:recalculateSubviewPositions()
end

