module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuBar" "lost.guiro.view.View" {}

-- menuBarItemStyleParams must be set in style
-- itemPadding: will be added to width, so padding = item title margin*2
-- itemLeftOffset: x offset of first item
function MenuBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menubar"
  self.menuBarItemStyleParams.size = args.size
  self._menuBarItems = {}
  self:items(t.items or {})
  self._hoverMode = false
  self._itemPressTime = 0
  self.clickDelta = 0.400
  self._activeItem = nil
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.MenuBar")          
end

function MenuBar:rebuildMenuBarItems()
  -- delete old MenuBarItems
  for k,v in ipairs(self._menuBarItems) do 
    self:removeSubview(v)
  end
  self._menuBarItems = {}
  
  
  -- create new ones
  local xoffset = self.itemLeftOffset
  for k,item in ipairs(self._items) do
    -- build style table that's used for construction of the items
    -- copy style params that were sent from MenuBar style
    local mbistyle = {}
    for k,v in pairs(self.menuBarItemStyleParams) do
      mbistyle[k] = v
    end
    mbistyle.bounds = {100,0,100,22}
    for k,v in pairs(item) do
      mbistyle[k] = v
    end
    local mbi = lost.guiro.view.MenuBarItem(mbistyle)
    mbi.delegate = self
    table.insert(self._menuBarItems, mbi)
    self:addSubview(mbi)
    local m = lost.font.render(item.title, mbi.textLayer:font(), false,0)
    local w = m.size.width+self.itemPadding
    mbi:width(w)
    mbi:x(xoffset)
    mbi:y("center")
    xoffset = xoffset+w
  end
end

function MenuBar:items(...)
  if arg.n >= 1 then
    self._items = arg[1] or {}
    self:rebuildMenuBarItems()
  else
    return self._items
  end
end

-- MenuBarItem delegate methods

function MenuBar:itemPressed(mbi)
  if self._hoverMode then
    self._itemPressTime = 0
  else
    self._hoverMode = true
    self._itemPressTime = lost.platform.currentTimeSeconds()
  end
end

function MenuBar:itemReleased(mbi)
  local d = 0
  if self._itemPressTime == 0 then
    self._hoverMode = false
  else
    local t = lost.platform.currentTimeSeconds()
    d = t - self._itemPressTime
    if d > self.clickDelta then
      self._hoverMode = false
    end
  end
end

function MenuBar:itemShouldHighlight()
  return self._hoverMode
end

function MenuBar:itemActive(mbi)
  if self._activeItem then
    if self._activeItem ~= mbi then
      self._activeItem:highlight(false)
      self._activeItem:closeMenu()
    end
  end
  self._activeItem = mbi
end

function MenuBar:itemInactive(mbi)
  if self._activeItem == mbi then
    self._activeItem = nil
  end
end

function MenuBar:menuBarItemExternalCloseRequest(item)
  self._hoverMode = false
end
