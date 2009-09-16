module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Scrollbar")

local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
local xright = lost.guiro.xright
local xleft = lost.guiro.xleft
local ytop = lost.guiro.ytop
local ybottom = lost.guiro.ybottom
local xcenter = lost.guiro.xcenter
local ycenter = lost.guiro.ycenter
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

lost.common.Class "lost.guiro.ScrollView" "lost.guiro.View" {}

function ScrollView:constructor()
	lost.guiro.View.constructor(self)

  -- scroll handling
	self:addEventListener(event.MouseEvent.MOUSE_SCROLL, function(event)
	  if event.scrollDelta.x ~= 0 then
	    self._horizontalScrollbar:value(self._horizontalScrollbar:value() - 5.0*event.scrollDelta.x)
	  end
	  if event.scrollDelta.y ~= 0 then
	    self._verticalScrollbar:value(self._verticalScrollbar:value() - 5.0*event.scrollDelta.y)
	  end
	end)
end

function ScrollView:contentView(view)
  if view ~= nil then
  	self._contentView = view

    -- enable scissoring
    self._contentView.scissorNode.active = true
    self._contentView.scissorRectNode.active = true
    self._contentView.disableScissorNode.active = true

  	lost.guiro.View.addSubview(self, self._contentView)
  else
    return self._contentView
  end
end

function ScrollView:contentViewBounds(bounds)
  if self._contentView ~= nil then
    if bounds ~= nil then
      self._contentView.bounds = bounds
      self._contentView:needsLayout()
    else
      return self._contentView.bounds
    end
  end
end

function ScrollView:horizontalScrollbar(scrollbar)
  if scrollbar ~= nil then
    self._horizontalScrollbar = scrollbar
    self._horizontalScrollbar.bounds = Bounds(xleft(), ybottom(), wrel(1, -15), habs(15))
    self._horizontalScrollbar:addEventListener("scrollbarChange", function(event)
      self._contentView.bounds.x = xleft({abs = -event.target:value()})
      self._contentView:needsLayout()
    end)
    lost.guiro.View.addSubview(self, self._horizontalScrollbar)
  else
    return self._horizontalScrollbar
  end
end

function ScrollView:verticalScrollbar(scrollbar)
  if scrollbar ~= nil then
    self._verticalScrollbar = scrollbar
    self._verticalScrollbar.bounds = Bounds(xright(), ybottom({abs=15}), wabs(15), hrel(1, -15))
    self._verticalScrollbar:addEventListener("scrollbarChange", function(event)
      self._contentView.bounds.y = ytop({abs = event.target:value()})
      self._contentView:needsLayout()
    end)
    lost.guiro.View.addSubview(self, self._verticalScrollbar)
  else
    return self._verticalScrollbar
  end
end

function ScrollView:addSubview(newview, pos)
  self._contentView:addSubview(newview, pos)
end

function ScrollView:applyLayout()
  -- update scrollbars
  local contentRect = self._contentView:globalRect()
  local size = contentRect.width - self.currentGlobalRect.width
  self._horizontalScrollbar:max(size)
  -- add scrollbar dimensions to scroll area
  if not self._horizontalScrollbar:hidden() then
    size = contentRect.height - self.currentGlobalRect.height + self._horizontalScrollbar:globalRect().height
  else
    size = contentRect.height - self.currentGlobalRect.height
  end
  self._verticalScrollbar:max(size)
  -- add scrollbar dimensions to scroll area
  if not self._verticalScrollbar:hidden() then
    size = contentRect.width - self.currentGlobalRect.width + self._verticalScrollbar:globalRect().width
    self._horizontalScrollbar:max(size)
  end

  -- apply scissor rect
  local gr = lost.math.Rect(self.currentGlobalRect.x, self.currentGlobalRect.y,
      self.currentGlobalRect.width, self.currentGlobalRect.height)
  if not self._horizontalScrollbar:hidden() then
    gr.y = gr.y + self._horizontalScrollbar:globalRect().height
    gr.height = gr.height - self._horizontalScrollbar:globalRect().height
  end
  if not self._verticalScrollbar:hidden() then
    gr.width = gr.width - self._verticalScrollbar:globalRect().width
  end
  self._contentView.scissorRect = gr

  lost.guiro.View.applyLayout(self)
end
