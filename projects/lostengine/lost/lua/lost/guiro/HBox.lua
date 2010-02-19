module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.HBox" "lost.guiro.View" {}

function HBox:constructor()
	lost.guiro.View.constructor(self)
	self._mode = "spread" -- spread or stack
	self._valign = "top" -- only used if mode != spread, 'top' 'center' or 'bottom'
	self._halign = "center"
	self._spacing = 0 -- only used when mode is stacked
end

function HBox:addSubview(subview)
  lost.guiro.View.addSubview(self, subview)
  self:needsLayout()
end

function HBox:removeSubview(subview)
  lost.guiro.View.removeSubview(self, subview)
  self:needsLayout()
end

function HBox:removeAllSubviews()
  lost.guiro.View.removeAllSubviews(self)
  self:needsLayout()
end

-- returns the sum of the current width of all subviews
function HBox:calculateSubviewWidth()
  local result = 0
  for k,view in pairs(self.subviews) do
    local gr = view.rect
    result = result + gr.width
  end
  return result
end

function HBox:updateVerticalAlignment(targetView)
  if self._valign == "center" then
    targetView.bounds.y = lost.guiro.ycenter()
  elseif self._valign == "top" then
    targetView.bounds.y = lost.guiro.ytop()
  elseif self._valign == "bottom" then
    targetView.bounds.y = lost.guiro.ybottom()
  else
    log.warn("unknown vertical alignment in HBox: '"..self._halign.."'")
  end
end

function HBox:updateSpread()
	local numViews = #(self.subviews)
	local relfactor = 1/(numViews-1)
	local f = 1
	for k = #self.subviews,1,-1 do
	  local view = self.subviews[k]
		view.bounds.x = lost.guiro.xcenter{rel=f}		
		f = f- relfactor
		self:updateVerticalAlignment(view)
		view:needsLayout()
	end
end

function HBox:updateStack()
  local subviewWidth = self:calculateSubviewWidth()
  local totalWidth = subviewWidth + (self._spacing*(#self.subviews-1))
  local xoffset = 0
  local gr = self.rect
  if self._halign == "center" then
    xoffset = (gr.width - totalWidth)/2 + totalWidth
  elseif self._halign == "left" then
    xoffset = 0 + totalWidth
  elseif self._halign == "right" then
    xoffset = gr.width 
  else
    log.warn("illegal horizontal alignment for HBox '"..self._halign.."'")
  end
  
  local currentOffset = xoffset
	for k = #self.subviews,1,-1 do
    local view = self.subviews[k]
    gr = view.rect
    currentOffset = currentOffset - gr.width
    view.bounds.x = lost.guiro.xabs(currentOffset)
    view:needsLayout()
		self:updateVerticalAlignment(view)    
    currentOffset = currentOffset - self._spacing
  end
end

function HBox:afterLayout()
  if self._mode == "spread" then
    self:updateSpread()
  elseif self._mode == "stack" then
    self:updateStack()
  else
    log.warn("can't update HBox for mode '"..self._mode.."'")
  end
end

function HBox:mode(val)
	if val ~= nil then
		self._mode = val
    self:needsLayout()
	else
		return self._mode
	end
end

function HBox:valign(val)
	if val ~= nil then
		self._valign = val
    self:needsLayout()
	else
		return self._valign
	end
end

function HBox:halign(val)
	if val ~= nil then
		self._halign = val
    self:needsLayout()
	else
		return self._halign
	end
end

function HBox:spacing(val)
	if val ~= nil then
		self._spacing = val
    self:needsLayout()
	else
		return self._spacing
	end
end
