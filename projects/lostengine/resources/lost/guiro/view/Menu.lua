module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Menu" "lost.guiro.view.Window" {}

local Color = lost.common.Color

-- optional attributes:
-- * delegate: will receive additional messages from Menu
-- * leftMargin
-- * rightMargin
-- * topMargin
-- * bottomMargin
-- * separatorHeight
-- * createSeparatorLayerFunc
-- FIXME: MenuItem class as parameter as well?
function Menu:constructor(args)
  log.debug("--")
  local t = args or {}
  
  for k,v in ipairs(t) do
    log.debug(tostring(k))
  end
  
  self._itemViews = {}
  self._separatorLayers = {}
  self.delegate = t.delegate or nil
  self.leftMargin = t.leftMargin or 0
  self.rightMargin = t.rightMargin or 0
  self.topMargin = t.topMargin or 0
  self.bottomMargin = t.bottomMargin or 0
  self.createSeparatorLayerFunc = t.createSeparatorLayerFunc or function() return lost.guiro.layer.Rect{bounds={0,0,"1",2},color=Color(0,0,0)} end
  self.separatorHeight = t.separatorHeight or 2
  lost.guiro.view.Window.constructor(self, args)
  self.id = t.id or "menu"
  if t.items then self:rebuildItems(args.items) end
end

function Menu:rebuildItems(t)
  log.debug("--")
  -- remove old menuitem views and layers
  for k,v in ipairs(self._itemViews) do
    self:removeSubview(v)
  end
  for k,v in ipairs(self._separatorLayers) do
    self.layer:removeSublayer(v)
  end
  self._itemViews = {}
  self._separatorLayers = {}
  
  -- reverse items so they're visualy constructed in teh same order as they're written in the text file
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
      mi = lost.guiro.view.MenuItem(v)
      self:addSubview(mi)
      table.insert(self._itemViews, mi)
      log.debug("/// "..mi.contentHeight)
      mi:height(mi.contentHeight)
      mi:width(mi.contentWidth)
      mi:x(self.leftMargin)
      mi:y(yoffset)
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
  log.debug("menuwidth: "..menuWidth.." height "..menuHeight)
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
  lost.guiro.windowManager():closeMenu(self)
end

function Menu:externalCloseRequest(event)
  if self.delegate then
    return self.delegate:menuExternalCloseRequest(self, event)
  else
    return true
  end
end