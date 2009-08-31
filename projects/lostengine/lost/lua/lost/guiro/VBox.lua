module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.VBox" "lost.guiro.View" {}

function VBox:constructor()
	lost.guiro.View.constructor(self)
	self._mode = "spread" -- spread or stack
	self._valign = "top" -- only used if mode != spread
	self._halign = "center"
	self._spacing = 0 -- only used when mode is stacked
	
	self.deferredRecalculate = function() self:recalculateSubviewPositions() end
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
		view:needsLayout()
	end
--	log.debug("end ---")
end

function VBox:addSubview(newview, pos)
	lost.guiro.View.addSubview(self, newview, pos)
--	self:recalculateSubviewPositions()
  callLater(self.deferredRecalculate)
end

function VBox:mode(val)
	if val ~= nil then
		self._mode = val
	else
		return self._mode
	end
end

function VBox:valign(val)
	if val ~= nil then
		self._valign = val
	else
		return self._valign
	end
end

function VBox:halign(val)
	if val ~= nil then
		self._halign = val
	else
		return self._halign
	end
end

function VBox:spacing(val)
	if val ~= nil then
		self._spacing = val
	else
		return self._spacing
	end
end
