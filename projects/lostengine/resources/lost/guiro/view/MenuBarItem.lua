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
  local t = args or {}
  self.id = t.id or "menubaritem"
  self:title(t.title or "")
  self:highlight(false)
  
  self:addEventListener("mouseDown", function(event) self:mouseDown(event) end)
  self:addEventListener("mouseUpInside", function(event) self:mouseUpInside(event) end)
  self:addEventListener("mouseUpOutside", function(event) self:mouseUpOutside(event) end)
  self:addEventListener("mouseEnter", function(event) self:mouseEnter(event) end)
  self:addEventListener("mouseLeave", function(event) self:mouseLeave(event) end)
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

function MenuBarItem:title(v)
  self.textLayer:text(v)
end

function MenuBarItem:mouseDown(event) 
  self:highlight(true)
  self.delegate:itemPressed(self)
  self.delegate:itemActive(self)
end

function MenuBarItem:mouseUpInside(event) 
  self:highlight(false)  
  self.delegate:itemReleased(self)
  self.delegate:itemInactive(self)
end

function MenuBarItem:mouseUpOutside(event) 
  self:highlight(false)  
  self.delegate:itemReleased(self)
  self.delegate:itemInactive(self)
end

function MenuBarItem:mouseEnter(event) 
  log.debug(event.type.." "..event.target.id)
  if self.delegate:itemShouldHighlight() then
    self:highlight(true)
    self.delegate:itemActive(self)
  end
end

function MenuBarItem:mouseLeave(event) 
  log.debug(event.type.." "..event.target.id)
  if self._highlighted then 
    self:highlight(false)
    self.delegate:itemInactive(self)
  end
end

