module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.PopUpButton" "lost.guiro.view.Button" {}

-- must be set from style:
-- * menuTheme
-- * menuStyle
function PopUpButton:constructor(args)
  lost.guiro.view.Button.constructor(self, args)
  local t = args or {}
  self.id = t.id or "popupbutton"
  local ms = {}
  ms.theme = self.menuTheme
  ms.style = self.menuStyle
  local incoming = t.menu or {}
  for k,v in pairs(incoming) do 
    ms[k] = v
  end
  self.menu = lost.guiro.view.Menu(ms)
  self:addEventListener("buttonClick", function(event) self:buttonClick(event) end)
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

-- Menu delegate methods

function PopUpButton:menuItemSelected(menu, indexPath)
  self.menu:close()
  local mi = self.menu:menuItemForIndexPath(indexPath)
  self:title(mi:title())
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