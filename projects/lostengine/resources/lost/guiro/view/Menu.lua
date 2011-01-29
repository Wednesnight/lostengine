module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Menu" "lost.guiro.view.Window" {}

function Menu:constructor(args)
  lost.guiro.view.Window.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menu"
end
