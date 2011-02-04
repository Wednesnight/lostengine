module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuBar" "lost.guiro.view.View" {}

-- menuBarItemStyleParams must be set in style
-- itemPadding: will be added to width, so padding = item title margin*2
-- itemLeftOffset: x offset of first item
function MenuBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menubar"
  
  self._menuBarItems = {}
  self.buttonClickHandler = function(event) self:menuBarItemClick(event) end
  self.mouseDownHandler = function(event) self:menuBarMouseDown(event) end
  self.mouseUpInsideHandler = function(event) self:menuBarMouseUpInside(event) end
  self.mouseUpOutsideHandler = function(event) self:menuBarMouseUpOutside(event) end
  self:items(t.items or {})
  self._hoverMode = false
  self._itemPressTime = 0
  self.clickDelta = 0.400
  self._activeItem = nil
end

function MenuBar:menuBarItemClick(event)
  log.debug(event.type.." "..event.target.id)
end

function MenuBar:menuBarMouseDown(event)
  log.debug(event.type.." "..event.target.id)
  self:releaseAllItems()
  event.target:highlight(true)
  log.debug("down time: "..tostring(lost.platform.currentTimeSeconds()))
end

function MenuBar:menuBarMouseUpInside(event)
  log.debug(event.type.." "..event.target.id)
end

function MenuBar:menuBarMouseUpOutside(event)
  log.debug(event.type.." "..event.target.id)
end

function MenuBar:releaseAllItems()
  for k,barItem in ipairs(self._menuBarItems) do
    barItem:highlight(false)
  end
end

function MenuBar:rebuildMenuBarItems()
  -- delete old MenuBarItems
  for k,v in ipairs(self._menuBarItems) do 
    self:removeSubview(v)
  end
  self._menuBarItems = {}
  
  -- build style table that's used for construction of the items
  -- copy style params that were sent from MenuBar style
  local mbistyle = {}
  for k,v in ipairs(self.menuBarItemStyleParams) do
    mbistyle[k] = v
  end
  
  -- create new ones
  local xoffset = self.itemLeftOffset
  for k,item in ipairs(self._items) do
    mbistyle.bounds = {100,0,100,22}
    mbistyle.title = item.title
    mbistyle.menu = item
    local mbi = lost.guiro.view.MenuBarItem(mbistyle)
    mbi.delegate = self
    table.insert(self._menuBarItems, mbi)
--    mbi:addEventListener("buttonClick", self.buttonClickHandler )
--    mbi:addEventListener("mouseDown", self.mouseDownHandler )
--    mbi:addEventListener("mouseUpInside", self.mouseUpInsideHandler )
--    mbi:addEventListener("mouseUpOutside", self.mouseUpOutsideHandler )
    self:addSubview(mbi)
    local m = lost.font.render(item.title, mbi.textLayer:font(), false)
    local w = m.size.width+self.itemPadding
    log.debug("!! text width "..tostring(w))
    mbi:width(w)
    mbi:x(xoffset)
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
--    log.debug("!! preparing switch off from hover mode")
  else
    self._hoverMode = true
    self._itemPressTime = lost.platform.currentTimeSeconds()
  end
--  log.debug("PRESSED "..mbi.id.." "..tostring(self._itemPressTime))
end

function MenuBar:itemReleased(mbi)
--  log.debug("/////// press time "..tostring(self._itemPressTime))
  if self._itemPressTime == 0 then
    self._hoverMode = false
--    log.debug("!!!!!!!!!!!!!!!! switch off from hover!")
  else
    local t = lost.platform.currentTimeSeconds()
    local d = t - self._itemPressTime
    if d > self.clickDelta then
      self._hoverMode = false
    end
  end
--  log.debug("RELEASED "..mbi.id.." clickdelta "..tostring(d))  
end

function MenuBar:itemShouldHighlight()
  return self._hoverMode
end

function MenuBar:itemActive(mbi)
--  log.debug("active "..mbi.id)
  if self._activeItem then
    if self._activeItem ~= mbi then
      self._activeItem:highlight(false)
    end
  end
  self._activeItem = mbi
end

function MenuBar:itemInactive(mbi)
  if self._activeItem == mbi then
    self._activeItem = nil
  end
end

