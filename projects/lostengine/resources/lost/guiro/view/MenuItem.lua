module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuItem" "lost.guiro.view.View" {}

-- must be set from style:
-- * checkmarkLayer
-- * submenuLayer (arrow for submenu)
-- * textLayer (for title)
-- * contentHeight
-- * checkmarkWidth
-- * submenuWidth
-- * checkmarkTextDistance 
-- * textSubmenuDistance
-- * normalTextColor
-- * highlightTextColor
-- can be set from whoever requires delegate calls
-- * delegate: will receive enter/leave/mouseUp-Events
function MenuItem:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menuitem"
  self:title(t.title or " ")
  self:checked(t.checked or false)
  
  self:addEventListener("mouseEnter", function(event) self:mouseEnter(event) end)
  self:addEventListener("mouseLeave", function(event) self:mouseLeave(event) end)
  self:addEventListener("mouseUpInside", function(event) self:mouseUpInside(event) end)
  self:highlight(false)
end

function MenuItem:highlight(...)
  if arg.n >= 1 then
    if arg[1] then
      self.textLayer:color(self.highlightTextColor)
    else
      self.textLayer:color(self.normalTextColor)
    end
  end
end

function MenuItem:mouseEnter(event)
  if self.delegate then
    self.delegate:menuItemEntered(self)
  end
end

function MenuItem:mouseLeave(event)
  if self.delegate then
    self.delegate:menuItemLeft(self)
  end
end

function MenuItem:mouseUpInside(event)
  if self.delegate then
    self.delegate:menuItemClicked(self)
  end
end

function MenuItem:updateContentWidth()
  self.contentWidth = self.checkmarkWidth
                      + self.checkmarkTextDistance 
                      + self.textWidth
                      + self.textSubmenuDistance
                      + self.submenuWidth
end

function MenuItem:updateTextWidth()
  local m = lost.font.render(self.textLayer:text(), self.textLayer:font(), false)
  self.textWidth = m.size.width  
end

function MenuItem:title(v)
  self.textLayer:text(v)
  self:updateTextWidth()
  self:updateContentWidth()
end

function MenuItem:checked(...)
  if arg.n >= 1 then 
    self.checkmarkLayer:hidden(not arg[1])
  else
    return not self.checkmarkLayer:hidden()
  end
end
