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
  self.mouseDownHandler = function(event) self:mouseDown(event) end
  self.mouseUpHandler = function(event) self:mouseUp(event) end
  self.mouseEnterHandler = function(event) self:mouseEnter(event) end
  self.mouseLeaveHandler = function(event) self:mouseLeave(event) end
end

function MenuBarItem:highlight(v)
  if v then
    self.highlightLayer:show()
    self.textLayer:color(self.highlightedTextColor)
  else
    self.highlightLayer:hide()
    self.textLayer:color(self.normalTextColor)
  end
end

function MenuBarItem:title(v)
  self.textLayer:text(v)
end

function MenuBarItem:mouseDown(event) 
  self:highlight(true)
end

function MenuBarItem:mouseUpInside(event) 
  self:highlight(false)  
end

function MenuBarItem:mouseUpOutside(event) 
  self:highlight(false)  
end

function MenuBarItem:mouseEnter(event) 
  log.debug(event.type.." "..event.target.id)
end

function MenuBarItem:mouseLeave(event) 
  log.debug(event.type.." "..event.target.id)
end

