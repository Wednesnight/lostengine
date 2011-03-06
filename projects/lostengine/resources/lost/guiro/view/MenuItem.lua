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
  self._styleSize = args.size
  self:title(t.title or " ")
  self:checked(t.checked or false)
  
  self:addEventListener("mouseEnter", function(event) self:mouseEnter(event) end)
  self:addEventListener("mouseLeave", function(event) self:mouseLeave(event) end)
  self:addEventListener("mouseUpInside", function(event) self:mouseUpInside(event) end)
  self:highlight(false)
  if t.menu then
    local mdef = {}
    for k,v in pairs(t.menu) do
      mdef[k] = v
    end
    if mdef.size == nil then
      mdef.size = self._styleSize
    end
    local m = lost.guiro.view.Menu(mdef)
    self:menu(m)
  else
    self:submenuIcon(false)
  end
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.MenuItem")          
end

function MenuItem:menu(...)
  if arg.n >= 1 then
    local m = arg[1]
    self._menu = m
    if m ~= nil then
      self:submenuIcon(true)
    else
      self:submenuIcon(false)
    end
  else
    return self._menu
  end
end

function MenuItem:submenuIcon(...)
  if arg.n >= 1 then 
    self.submenuLayer:hidden(not arg[1])
  else
    return not self.submenuLayer:hidden()
  end
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
    self.delegate:menuItemEntered(event,self)
  end
end

function MenuItem:mouseLeave(event)
  if self.delegate then
    self.delegate:menuItemLeft(event,self)
  end
end

function MenuItem:mouseUpInside(event)
  if self.delegate then
    self.delegate:menuItemClicked(event,self)
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

function MenuItem:title(...)
  if arg.n >= 1 then
    self.textLayer:text(arg[1])
    self:updateTextWidth()
    self:updateContentWidth()
  else
    return self.textLayer:text()
  end
end

function MenuItem:checked(...)
  if arg.n >= 1 then 
    self.checkmarkLayer:hidden(not arg[1])
  else
    return not self.checkmarkLayer:hidden()
  end
end
