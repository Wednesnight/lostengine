module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.HBox" "lost.guiro.View" {}

function HBox:constructor()
	lost.guiro.View.constructor(self)
end

function HBox:updateLayout(forceUpdate)
	if forceUpdate or self.dirtyLayout
		self.updateLayout(forceUpdate)
		-- relayout all subviews
--[[		local xoffset = 0
		for k,v in pairs(self.subviews)
			v.bounds.x = lost.guiro.xabs()
		end]]--
	end
end


