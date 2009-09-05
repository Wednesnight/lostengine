module("lost.guiro", package.seeall)

require("lost.guiro.View")

lost.common.Class "lost.guiro.VBox" "lost.guiro.View" {}

function VBox:constructor()
	lost.guiro.View.constructor(self)
	self._mode = "spread" -- spread or stack
	self._valign = "top" -- only used if mode != spread, 'top' 'center' or 'bottom'
	self._halign = "center"
	self._spacing = 0 -- only used when mode is stacked
	
	self.deferredUpdate = function() self:update() end
end

-- returns the sum of the current heights of all subviews
function VBox:calculateSubviewHeight()
  local result = 0
  for k,view in pairs(self.subviews) do
    local gr = view:globalRect()
    result = result + gr.height
  end
  return result
end

function VBox:updateHorizontalAlignment(targetView)
  if self._halign == "center" then
    targetView.bounds.x = xcenter()
  elseif self._halign == "left" then
    targetView.bounds.x = xleft()
  elseif self._halign == "right" then
    targetView.bounds.x = xright()
  else
    log.warn("unknown horizontal alignment in VBox: '"..self._halign.."'")
  end
end

function VBox:updateSpread()
--	log.debug("start ---")
	local numViews = #(self.subviews)
	local relfactor = 1/(numViews-1)
--	log.debug("relfactor "..relfactor)
	local f = 1
	for k,view in pairs(self.subviews) do
		log.debug("f "..f)
		view.bounds.y = lost.guiro.ycenter{rel=f}		
		f = f- relfactor
		self:updateHorizontalAlignment(view)
		view:needsLayout()
	end
--	log.debug("end ---")
end

function VBox:updateStack()
  local subviewHeight = self:calculateSubviewHeight()
  local totalHeight = subviewHeight + (self._spacing*(#self.subviews-1))
  local yoffset = 0
  local gr = self:globalRect()
  if self._valign == "center" then
    yoffset = (gr.height - totalHeight)/2 + totalHeight
  elseif self._valign == "bottom" then
    yoffset = 0 + totalHeight
  elseif self._valign == "top" then
    yoffset = gr.height 
  else
    log.warn("illegal vertical alignment for VBox '"..self._valign.."'")
  end
  
  local currentOffset = yoffset
  for k,view in pairs(self.subviews) do
    gr = view:globalRect()
    currentOffset = currentOffset - gr.height
    view.bounds.y = lost.guiro.yabs(currentOffset)
		self:updateHorizontalAlignment(view)    
    currentOffset = currentOffset - self._spacing
  end
end

function VBox:update()
  if self._mode == "spread" then
    self:updateSpread()
  elseif self._mode == "stack" then
    self:updateStack()
  else
    log.warn("can't update VBox for mode '"..self._mode.."'")
  end
end

function VBox:addSubview(newview, pos)
	lost.guiro.View.addSubview(self, newview, pos)
  callLater(self.deferredUpdate)
end

function VBox:updateLayout(forceUpdate)
  local doUpdate = forceUpdate or self.dirtyLayout
  lost.guiro.View.updateLayout(self, forceUpdate)
  -- FIXME only own rect should be update here, subviews are managed by HBox update function
  if doUpdate then callLater(self.deferredUpdate) end
end

function VBox:mode(val)
	if val ~= nil then
		self._mode = val
    callLater(self.deferredUpdate)
	else
		return self._mode
	end
end

function VBox:valign(val)
	if val ~= nil then
		self._valign = val
    callLater(self.deferredUpdate)
	else
		return self._valign
	end
end

function VBox:halign(val)
	if val ~= nil then
		self._halign = val
    callLater(self.deferredUpdate)
	else
		return self._halign
	end
end

function VBox:spacing(val)
	if val ~= nil then
		self._spacing = val
    callLater(self.deferredUpdate)
	else
		return self._spacing
	end
end
