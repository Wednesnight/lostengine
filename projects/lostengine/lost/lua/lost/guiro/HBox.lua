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

	self.deferredUpdate = function() self:update() end
end

-- returns the sum of the current width of all subviews
function HBox:calculateSubviewWidth()
  local result = 0
  for k,view in pairs(self.subviews) do
    local gr = view:globalRect()
    result = result + gr.width
  end
  return result
end

function HBox:updateVerticalAlignment(targetView)
  if self._halign == "center" then
    targetView.bounds.y = lost.guiro.ycenter()
  elseif self._halign == "top" then
    targetView.bounds.y = lost.guiro.ytop()
  elseif self._halign == "bottom" then
    targetView.bounds.y = lost.guiro.ybottom()
  else
    log.warn("unknown vertical alignment in HBox: '"..self._halign.."'")
  end
end

function HBox:updateSpread()
--	log.debug("start ---")
	local numViews = #(self.subviews)
	local relfactor = 1/(numViews-1)
--	log.debug("relfactor "..relfactor)
	local f = 1
	for k,view in pairs(self.subviews) do
		log.debug("f "..f)
		view.bounds.x = lost.guiro.xcenter{rel=f}		
		f = f- relfactor
		self:updateVerticalAlignment(view)
		view:needsLayout()
	end
--	log.debug("end ---")
end

function HBox:updateStack()
	log.debug("HBox updateStack")
  local subviewWidth = self:calculateSubviewWidth()
  local totalWidth = subviewWidth + (self._spacing*(#self.subviews-1))
  local xoffset = 0
  local gr = self:globalRect()
  if self._valign == "center" then
    xoffset = (gr.width - totalWidth)/2 + totalWidth
  elseif self._valign == "bottom" then
    xoffset = 0 + totalWidth
  elseif self._valign == "top" then
    xoffset = gr.width 
  else
    log.warn("illegal horizontal alignment for HBox '"..self._valign.."'")
  end
  
  local currentOffset = xoffset
  for k,view in pairs(self.subviews) do
    gr = view:globalRect()
    currentOffset = currentOffset - gr.width
    view.bounds.x = lost.guiro.xabs(currentOffset)
		self:updateVerticalAlignment(view)    
    currentOffset = currentOffset - self._spacing
  end
end

function HBox:update()
  if self._mode == "spread" then
    self:updateSpread()
  elseif self._mode == "stack" then
    self:updateStack()
  else
    log.warn("can't update HBox for mode '"..self._mode.."'")
  end
	self:needsLayout()
end

function HBox:addSubview(newview, pos)
	lost.guiro.View.addSubview(self, newview, pos)
  callLater(self.deferredUpdate)
end

function HBox:updateLayout(forceUpdate)
  local doUpdate = forceUpdate or self.dirtyLayout
  lost.guiro.View.updateLayout(self, forceUpdate)
  -- FIXME only own rect should be update here, subviews are managed by HBox update function
--  if doUpdate then callLater(self.deferredUpdate) end
end

function HBox:mode(val)
	if val ~= nil then
		self._mode = val
    callLater(self.deferredUpdate)
	else
		return self._mode
	end
end

function HBox:valign(val)
	if val ~= nil then
		self._valign = val
    callLater(self.deferredUpdate)
	else
		return self._valign
	end
end

function HBox:halign(val)
	if val ~= nil then
		self._halign = val
    callLater(self.deferredUpdate)
	else
		return self._halign
	end
end

function HBox:spacing(val)
	if val ~= nil then
		self._spacing = val
    callLater(self.deferredUpdate)
	else
		return self._spacing
	end
end
