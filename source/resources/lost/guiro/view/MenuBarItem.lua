module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuBarItem" "lost.guiro.view.View" {}

-- these attributes must be set from style:
-- * textLayer
-- * highlightLayer
-- * normalTextColor
-- * highlightedTextColor
-- must be set from MenuBar
-- * delegate: receives notifications about mouse up/down enter/leave and will be asked about highlight behaviour of item
function MenuBarItem:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  self._styleSize = args.size -- for menu creation
  local t = args or {}
  self.id = t.id or "menubaritem"
  self:title(t.title or "")
  if args.menu then self:menu(args.menu) end
  self:highlight(false)
  self:addEventListener("mouseDown", function(event) self:mouseDown(event) end)
  self:addEventListener("mouseUpInside", function(event) self:mouseUpInside(event) end)
  self:addEventListener("mouseUpOutside", function(event) self:mouseUpOutside(event) end)
  self:addEventListener("mouseEnter", function(event) self:mouseEnter(event) end)
  self:addEventListener("mouseLeave", function(event) self:mouseLeave(event) end)
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.MenuBarItem")          
end

function MenuBarItem:openMenu()
  self._menu:x(self.rect.x)
  local ny = self.rect.y-self._menu.rect.height
  self._menu:y(ny)
  self._menu:open()
end

function MenuBarItem:closeMenu()
  self._menu:close()
end

function MenuBarItem:highlight(v)
  if v then
    self._highlighted = true
    self.highlightLayer:show()
    self.textLayer:color(self.highlightedTextColor)
  else
    self._highlighted = false
    self.highlightLayer:hide()
    self.textLayer:color(self.normalTextColor)
  end
end

function MenuBarItem:menu(t)
  local params = {}
  params.style = self.menuStyle
  params.theme = self.menuTheme
  params.size = self._styleSize
  for k,v in pairs(t) do
    params[k] = v
  end
  self._menu = lost.guiro.view.Menu(params)
  self._menu.delegate = self
end

function MenuBarItem:title(v)
  self.textLayer:text(v)
end

function MenuBarItem:mouseDown(event) 
  self.delegate:itemPressed(self)
  if self.delegate:itemShouldHighlight() then
    self:highlight(true)
    self:openMenu()
    self.delegate:itemActive(self)
  end
end

function MenuBarItem:mouseUpInside(event) 
  self.delegate:itemReleased(self)
  if self.delegate:itemShouldHighlight() then
    self:highlight(true)
    self:openMenu()
    self.delegate:itemActive(self)
  else
    self:highlight(false)  
    self:closeMenu()
    self.delegate:itemInactive(self)
  end
end

function MenuBarItem:mouseUpOutside(event) 
  self.delegate:itemReleased(self)
  if self.delegate:itemShouldHighlight() then
    self:highlight(true)
    self:openMenu()
    self.delegate:itemActive(self)
  else
    self:highlight(false)  
    self:closeMenu()
    self.delegate:itemInactive(self)
  end
end

function MenuBarItem:mouseEnter(event) 
--  log.debug(event.type.." "..event.target.id.." shouldHghlight: "..tostring(self.delegate:itemShouldHighlight()))
  if self.delegate:itemShouldHighlight() then
    self:highlight(true)
    self:openMenu()
    self.delegate:itemActive(self)
  end
end

function MenuBarItem:mouseUnderItem(pos)
  local result = false
  
  if (pos.x >= self.rect.x) and (pos.x <= (self.rect.x+self.rect.width)) and (pos.y < self.rect.y) then
    result = true
  end
  
  return result
end

function MenuBarItem:mouseLeave(event) 
--  log.debug(event.type.." "..event.target.id)
  if self._highlighted and (not self:mouseUnderItem(event.pos)) then 
    self:highlight(false)
    self:closeMenu()
    self.delegate:itemInactive(self)
  end
end

-- Menu delegate methods

function MenuBarItem:menuItemSelected(menu, indexPath)
  self.delegate:itemReleased(self)
  self:highlight(false)  
  self:closeMenu()
  self.delegate:itemInactive(self)  
end

function MenuBarItem:menuExternalCloseRequest(menu, event)
  if not self:containsCoord(event.pos) then
    self:highlight(false)
  --  self.delegate:itemReleased(self)  
    self.delegate:menuBarItemExternalCloseRequest(self)
    self.delegate:itemInactive(self)
    return true
  else
    return false
  end
end