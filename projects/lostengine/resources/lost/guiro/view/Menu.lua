module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Menu" "lost.guiro.view.Window" {}

-- optional attributes:
-- * delegate: will receive additional messages from Menu
function Menu:constructor(args)
  lost.guiro.view.Window.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menu"
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