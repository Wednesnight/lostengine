module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuBar" "lost.guiro.view.View" {}

-- menuBarItemStyleParams must be set in style
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
  local i = 0
  for k,item in ipairs(self._items) do
    mbistyle.bounds = {i*100,0,100,22}
    mbistyle.title = item.title
    mbistyle.id = tostring(i+1)
    local mbi = lost.guiro.view.MenuBarItem(mbistyle)
    table.insert(self._menuBarItems, mbi)
    mbi:addEventListener("buttonClick", self.buttonClickHandler )
    mbi:addEventListener("mouseDown", self.mouseDownHandler )
    mbi:addEventListener("mouseUpInside", self.mouseUpInsideHandler )
    mbi:addEventListener("mouseUpOutside", self.mouseUpOutsideHandler )
    self:addSubview(mbi)
    i = i+1
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
