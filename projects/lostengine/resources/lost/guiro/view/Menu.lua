module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.Menu" "lost.guiro.view.View" {}

function Menu:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menu"
end
