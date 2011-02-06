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
function MenuItem:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menuitem"
  self:title(t.title or " ")
  self:checked(t.checked or false)
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
  log.debug("!! txt: '"..self.textLayer:text().."' "..tostring(m.size).." "..tostring(m.min).." "..tostring(m.max))
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
