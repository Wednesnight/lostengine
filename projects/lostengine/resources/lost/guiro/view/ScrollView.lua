module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollView" "lost.guiro.view.View" {}

local Vec2 = lost.math.Vec2

-- set by style:
-- * contentView
-- * verticalScrollbar
-- * horizontalScrollbar
-- * scrollbarWidth
--
-- add content as regular subviews, will be put into contentView.
-- set contentSize afterwards
--
function ScrollView:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollview"
  self:contentSize(t.contentSize or Vec2(100,100))
  -- move scrollbars so content is aligned topleft by default
  self.verticalScrollbar:value(1)
  self.horizontalScrollbar:value(0)
  self.horizontalScrollbar:addEventListener("valueChanged", function(event) self:scrollbarValueChanged(event) end)
  self.verticalScrollbar:addEventListener("valueChanged", function(event) self:scrollbarValueChanged(event) end)
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
    self:needsLayout()
  else
    return self._contentSize
  end
end

function ScrollView:showScrollbars(horizontal, vertical)
  if horizontal then
    self.horizontalScrollbar:hidden(false)
    if vertical then
      self.horizontalScrollbar:width({"1",-self.scrollbarWidth})
    else
      self.horizontalScrollbar:width("1")
    end
  else
    self.horizontalScrollbar:hidden(true)
  end
  
  if vertical then
    self.verticalScrollbar:hidden(false)    
    if horizontal then
      self.verticalScrollbar:height({"1",-self.scrollbarWidth})
      self.verticalScrollbar:y(self.scrollbarWidth)
    else
      self.verticalScrollbar:height("1")
      self.verticalScrollbar:y(0)
    end
  else
    self.verticalScrollbar:hidden(true)    
  end
end

function ScrollView:updateScrollbarVisibility()
  local showVertical = false
  local showHorizontal = false
  if self._contentSize.x > self.rect.width then
    showHorizontal = true
  end
  if self._contentSize.y > self.rect.height then
    showVertical = true
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
  if not self.verticalScrollbar:hidden() then
    hrange = hrange + self.scrollbarWidth
  end
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
  self.contentView:y(-voffset+vmod)

end

function ScrollView:updateLayout()
  lost.guiro.view.View.updateLayout(self)

  self:updateScrollbarVisibility()
  self:updateScrollbarVisibleRange()
  self:updateContentPosition()
end