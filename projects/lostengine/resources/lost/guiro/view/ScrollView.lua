module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollView" "lost.guiro.view.View" {}

local Vec2 = lost.math.Vec2

-- set by style:
-- * contentView
-- * verticalScrollbar
-- * horizontalScrollbar
-- * cornerView (missing piece when two scrollbars are present)
-- * scrollbarWidth
-- * cornerView
--
-- add content as regular subviews, will be put into contentView.
-- set contentSize afterwards
-- * alwaysShowCorner: set this to true if you want the bottom right corner view always to be shown, permanently insetting the scrollbars
function ScrollView:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollview"
  self:contentSize(t.contentSize or Vec2(100,100))
  if t.hasVerticalScrollbar ~= nil then
    self:hasVerticalScrollbar(t.hasVerticalScrollbar)
  else
    self:hasVerticalScrollbar(true)
  end
  if t.hasHorizontalScrollbar ~= nil then
    self:hasHorizontalScrollbar(t.hasHorizontalScrollbar)
  else
    self:hasHorizontalScrollbar(true)
  end

  if t.alwaysShowCorner ~= nil then 
    self:alwaysShowCorner(t.alwaysShowCorner) 
  else
    self:alwaysShowCorner(false) 
  end
  -- move scrollbars so content is aligned topleft by default
  self.verticalScrollbar:value(1)
  self.horizontalScrollbar:value(0)
  self.horizontalScrollbar:addEventListener("valueChanged", function(event) self:scrollbarValueChanged(event) end)
  self.verticalScrollbar:addEventListener("valueChanged", function(event) self:scrollbarValueChanged(event) end)
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.ScrollView")

	self:addEventListener("mouseScroll", function(event)
	  local x = math.max(math.min(self.horizontalScrollbar:value() - event.scrollDelta.x*2 / self._contentSize.x, 1), 0)
	  self.horizontalScrollbar:value(x)
	  local y = math.max(math.min(self.verticalScrollbar:value() + event.scrollDelta.y*2 / self._contentSize.y, 1), 0)
	  self.verticalScrollbar:value(y)
	  self:updateContentPosition()
	end)
end

function ScrollView:alwaysShowCorner(...)
  if arg.n >= 1 then
    self._alwaysShowCorner = arg[1]
    self:needsLayout()
  else
    return self._alwaysShowCorner
  end
end

function ScrollView:hasVerticalScrollbar(...)
  if arg.n >= 1 then
    self._hasVerticalScrollbar = arg[1]
    self:needsLayout()
  else
    return self._hasVerticalScrollbar
  end
end

function ScrollView:hasHorizontalScrollbar(...)
  if arg.n >= 1 then
    self._hasHorizontalScrollbar = arg[1]
    self:needsLayout()
  else
    return self._hasHorizontalScrollbar
  end
end

function ScrollView:scrollbarValueChanged(event)
  self:updateContentPosition()
end

function ScrollView:addSubview(view,forceSubview)
  if self.contentView and not forceSubview then
    self.contentView:addSubview(view)
  else
    lost.guiro.view.View.addSubview(self, view)
  end
end

function ScrollView:contentSize(...) -- Vec2
  if arg.n >= 1 then
    self._contentSize = arg[1]
    self.contentView:size(self._contentSize.x,self._contentSize.y)
    self:needsLayout()
  else
    return self._contentSize
  end
end

function ScrollView:showScrollbars(horizontal, vertical)
  if horizontal then
    self.horizontalScrollbar:hidden(false)
    if vertical or self._alwaysShowCorner then
      self.horizontalScrollbar:width({"1",-self.scrollbarWidth})
    else
      self.horizontalScrollbar:width("1")
    end
  else
    self.horizontalScrollbar:hidden(true)
  end
  
  if vertical then
    self.verticalScrollbar:hidden(false)    
    if horizontal or self._alwaysShowCorner then
      self.verticalScrollbar:height({"1",-self.scrollbarWidth})
      self.verticalScrollbar:y(self.scrollbarWidth)
    else
      self.verticalScrollbar:height("1")
      self.verticalScrollbar:y(0)
    end
  else
    self.verticalScrollbar:hidden(true)    
  end
  
  if not ((horizontal and vertical) or self._alwaysShowCorner) then
    self.cornerView:hidden(true)
  else
    self.cornerView:hidden(false)
  end
end

function ScrollView:updateScrollbarVisibility()
  local showVertical = false
  local showHorizontal = false
  if (self._contentSize.x > self.rect.width) and self:hasHorizontalScrollbar() then
    showHorizontal = true
    if (self._contentSize.y > (self.rect.height - self.scrollbarWidth)) and self:hasVerticalScrollbar() then
      showVertical = true
    end
  end
  if (self._contentSize.y > self.rect.height) and self:hasVerticalScrollbar() then
    showVertical = true
    if (self._contentSize.x > (self.rect.width - self.scrollbarWidth)) and self:hasHorizontalScrollbar() then
      showHorizontal = true
    end
  end
  self:showScrollbars(showHorizontal,showVertical)
end

function ScrollView:updateScrollbarVisibleRange()
  local vvr = 1
  local hvr = 1
  
  local hmod = 0
  local vmod = 0
  if not self.verticalScrollbar:hidden() then
    hmod = self.scrollbarWidth
  end
  if not self.horizontalScrollbar:hidden() then
    vmod = self.scrollbarWidth
  end
  
  hvr = (self.rect.width-hmod) / self._contentSize.x
  vvr = (self.rect.height-vmod) / self._contentSize.y
  self.verticalScrollbar:visibleRange(vvr)
  self.horizontalScrollbar:visibleRange(hvr)
end

function ScrollView:updateContentPosition()
  local hv = self.horizontalScrollbar:value()
  local hrange = math.max(self._contentSize.x - self.rect.width,0)
  local hmod = 0
  if not self.verticalScrollbar:hidden() then
    hmod = self.scrollbarWidth
  end
  hrange = hrange + hmod
  local hoffset = hrange*hv
  self.contentView:x(-hoffset)

  local vv = self.verticalScrollbar:value()
  local vrange = math.max(self._contentSize.y - self.rect.height,0)
  local vmod = 0
  if not self.horizontalScrollbar:hidden() then
    vmod = self.scrollbarWidth
  end
  vrange = vrange + vmod
  local voffset = vrange*vv
  local d = math.max((self.rect.height-self._contentSize.y),0)
  self.contentView:y(d-voffset+vmod)

end

-- checks if the content fits inside the view and resets the scrollbar positions 
-- to a default alignment
function ScrollView:alignScrollbars()
  if not (self._contentSize.x > self.rect.width) then -- content fits, align top
    self.horizontalScrollbar:value(0)
  end
  if not (self._contentSize.y > self.rect.height) then
    self.verticalScrollbar:value(1)
  end  
end

function ScrollView:updateLayout()
  lost.guiro.view.View.updateLayout(self)
  self:updateScrollbarVisibility()
  self:updateScrollbarVisibleRange()
  self:alignScrollbars()
  self:updateContentPosition()
end
