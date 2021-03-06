module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.PopUpButton" "lost.guiro.view.Button" {}

-- must be set from style:
-- * menuStyleParams
function PopUpButton:constructor(args)
  lost.guiro.view.Button.constructor(self, args)
  local t = args or {}
  self.id = t.id or "popupbutton"
  local ms = {} -- this will be the menu style for construction
  for k,v in pairs(self.menuStyleParams) do -- copy over params that were given by PopUpButton style
    ms[k] = v
  end
  local incoming = t.menu or {} -- find the incoming menu definition that should at least contain items, but can overwrite other params
  for k,v in pairs(incoming) do 
    ms[k] = v
  end
  self.menu = lost.guiro.view.Menu(ms)
  self:addEventListener("buttonClick", function(event) self:buttonClick(event) end)
  if t.selected then self:select(t.selected) end
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.PopUpButton")          
end

function PopUpButton:items(it)
  self.menu:items(it)
end

function PopUpButton:buttonClick(event)
  self.menu:x(self.rect.x)
  self.menu:y(self.rect.y+self.rect.height-self.menu.rect.height)
  self.menu.delegate = self
  self.menu:open()
end

function PopUpButton:select(indexPath)
  local mi = self.menu:menuItemForIndexPath(indexPath)
  if self.currentMenuItem ~= nil then
    self.currentMenuItem:checked(false)
  end
  if mi ~= nil then
    self:title(mi:title())
    mi:checked(true)
  else
    self:title("")
  end
  self.currentMenuItem = mi
end

-- Menu delegate methods

function PopUpButton:menuItemSelected(menu, indexPath)
  self.menu:close()
  self:select(indexPath)
  self:pushed(false)

  local event = lost.guiro.event.Event("menuItemSelected")
  event.bubbles = true
  event.target = self
  event.indexPath = indexPath
  self:dispatchEvent(event)    
end

function PopUpButton:menuExternalCloseRequest(menu, event)
  self.menu:close()
  self:pushed(false)
end

function PopUpButton:updateRect()
  lost.guiro.view.View.updateRect(self)
  self.menu:minWidth(self.rect.width)
end

