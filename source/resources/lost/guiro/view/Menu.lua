module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Menu" "lost.guiro.view.Window" {}

local Color = lost.common.Color

-- Menu
-- Sends menuItemSelected events with target = self and indexPath = {a,b,c} where a,b,c are 1-based indices into
-- the (sub)menu tree. Indices use the visual order, but the menuItems might be ordered different internally.
-- This is why you MUST use Menu:menuItemForIndexPath() to retrieve the appropriate MenuItem, even if it's only 
-- a single index. 
--
-- optional attributes:
-- * delegate: will receive additional messages from Menu
-- * leftMargin
-- * rightMargin
-- * topMargin
-- * bottomMargin
-- * minWidth
-- * separatorHeight
-- * createSeparatorLayerFunc
-- * highlightLayer
-- FIXME: MenuItem class as parameter as well?
function Menu:constructor(args)
  local t = args or {}  
  self._itemStyleSize = args.size
  self._itemViews = {}
  self._separatorLayers = {}
  self.delegate = t.delegate or nil
  self.leftMargin = t.leftMargin or 0
  self.rightMargin = t.rightMargin or 0
  self.topMargin = t.topMargin or 0
  self.bottomMargin = t.bottomMargin or 0
  self._minWidth = t.minWidth or 0
  self.createSeparatorLayerFunc = t.createSeparatorLayerFunc or function() return lost.guiro.layer.Rect{bounds={0,0,"1",2},color=Color(0,0,0)} end
  self.separatorHeight = t.separatorHeight or 2
  lost.guiro.view.Window.constructor(self, args)
  self.id = t.id or "menu"
  if t.items then self:rebuildItems(args.items) end
  self.highlightLayer:hide()
  self.currentMenuItem = nil -- will be set during highlighting
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.Menu")          
end

function Menu:rebuildItems(t)
  -- remove old menuitem views and layers
  for k,v in ipairs(self._itemViews) do
    self:removeSubview(v)
  end
  for k,v in ipairs(self._separatorLayers) do
    self.layer:removeSublayer(v)
  end
  self._itemViews = {}
  self._separatorLayers = {}
  
  -- reverse items so they're visualy constructed in the same order as they're written in the text file
  local reversedItems = {}
  for i=#t,1,-1 do
    table.insert(reversedItems, t[i])
  end
  -- create new items with contents from provided table
  -- assumes table of tables that are in MenuItem constructor format
  local mi = 0
  local sep = 0
  local yoffset = self.bottomMargin
  local menuHeight = 0
  local menuWidth = 0
  for k,v in ipairs(reversedItems) do
    if type(v) == "table" then
      local midef = {}
      for k2,v2 in pairs(v) do
        midef[k2] = v2
      end
      if midef.size == nil then midef.size = self._itemStyleSize end
      mi = lost.guiro.view.MenuItem(midef)
      self:addSubview(mi)
      table.insert(self._itemViews, mi)
      mi:height(mi.contentHeight)
      mi:width(mi.contentWidth)
      mi:x(self.leftMargin)
      mi:y(yoffset)
      mi.delegate = self
      menuWidth = math.max(menuWidth, mi.contentWidth)
      yoffset = yoffset + mi.contentHeight
      menuHeight = menuHeight + mi.contentHeight
    elseif (type(v) == "string") and (v == "-") then 
      sep = self.createSeparatorLayerFunc()
      table.insert(self._separatorLayers, sep)
      self.layer:addSublayer(sep)
      sep:y(yoffset)
      yoffset = yoffset + self.separatorHeight
      menuHeight = menuHeight +self.separatorHeight
    end
  end
  -- adjust item width to new maxwidth
  for k,v in pairs(self._itemViews) do
    v:width(menuWidth)
  end
  menuWidth = menuWidth + self.leftMargin + self.rightMargin
  menuHeight = menuHeight + self.topMargin + self.bottomMargin
  self:width(menuWidth)
  self:height(menuHeight)
end

function Menu:items(t)
  self:rebuildItems(t)
end

function Menu:open()
  lost.guiro.windowManager():openMenu(self)
end

function Menu:close()
  if self.currentMenuItem then
    local m = self.currentMenuItem:menu()
    if m then
      m:close()
    end
    self:disableHighlight(self.currentMenuItem)
    self.currentMenuItem = nil
  end
  lost.guiro.windowManager():closeMenu(self)
end

function Menu:externalCloseRequest(event)
  if self.delegate then
    return self.delegate:menuExternalCloseRequest(self, event)
  else
    return true
  end
end

function Menu:enableHighlight(menuItem)
  menuItem:highlight(true)
  local b = menuItem:bounds()
  self.highlightLayer:show()
  self.highlightLayer:x(0)
  self.highlightLayer:y(b.y)
  self.highlightLayer:width("1")
  self.highlightLayer:height(b.height)
end

function Menu:disableHighlight(menuItem)
  menuItem:highlight(false)
  self.highlightLayer:hide()
end

-- MenuItem delegate methods

function Menu:openMenuItemSubmenu(menuItem)
  local m = menuItem:menu()
  m:x(self.rect.x+self.rect.width)
  m:y(menuItem.rect.y+menuItem.rect.height-m.rect.height+m.topMargin)
  m.superMenu = self
  m:open()
end

function Menu:menuItemEntered(event, menuItem)
  if self.currentMenuItem and (self.currentMenuItem ~= menuItem) then
    self.currentMenuItem:highlight(false)
    local m = self.currentMenuItem:menu()
    if m then m:close() end
  end
  menuItem:highlight(true)
  self:enableHighlight(menuItem)
  if menuItem:menu() then
    self:openMenuItemSubmenu(menuItem)
  end
  self.currentMenuItem=menuItem
end

function Menu:menuItemLeft(event, menuItem)
  -- only close if mouse didn't go out to the right into a submenu
  if menuItem:menu() 
      and (event.pos.y>=menuItem.rect.y) 
      and (event.pos.y<(menuItem.rect.y+menuItem.rect.height))
      and (event.pos.x >= (menuItem.rect.x+menuItem.rect.width))
  then
    self:openMenuItemSubmenu(menuItem)
  else      
    menuItem:highlight(false)
    self:disableHighlight(menuItem)
    if menuItem:menu() then
      menuItem:menu():close()
    end
    self.currentMenuItem=nil
  end
end

function Menu:indexForMenuItem(menuItem)
  local result = 0
  for k,v in ipairs(self._itemViews) do
    if rawequal(v, menuItem) then
      result = (#self._itemViews) - k + 1
      break
    end
  end
  return result
end

function Menu:menuItemClicked(event, menuItem, indexPath)
--  log.debug("/// item selected "..menuItem.id)
  -- add currently clicked menuItem to path
  if indexPath == nil then
    indexPath = {}
  end
  table.insert(indexPath, self:indexForMenuItem(self.currentMenuItem))

  if self.superMenu then
--    log.debug("delegating to supermenu, current item "..tostring(self.currentMenuItem))
    self.superMenu:menuItemClicked(event, self.currentMenuItem, indexPath)
  else
    -- reverse indexPath since we built it traversing upwards
    local reversed = {}
    for k,v in ipairs(indexPath) do
      table.insert(reversed, 1,v)
    end
    indexPath = reversed
--    log.debug("!!! indexPath for click")
--    for k,v in ipairs(indexPath) do
--      log.debug(v)
--    end
    self:dispatchSelectedEvent(indexPath)
    if self.delegate then 
      self.delegate:menuItemSelected(self, indexPath)
    end
  end
end

function Menu:dispatchSelectedEvent(indexPath)
  local event = lost.guiro.event.Event("menuItemSelected")
  event.bubbles = true
  event.target = self
  event.indexPath = indexPath
  self:dispatchEvent(event)    
end

function Menu:menuItemForIndexPath(indexPath)
  local currentMenu = self
  local result = 0
  for k,v in ipairs(indexPath) do
    result = currentMenu._itemViews[#currentMenu._itemViews-v+1]
    currentMenu = result:menu()
  end
  return result
end

function Menu:minWidth(w)
  self._minWidth = w
  self:needsLayout()
end

function Menu:updateRect()
  lost.guiro.view.View.updateRect(self)
  self.rect.width = math.max(self._minWidth, self.rect.width)
end

