module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuItem" "lost.guiro.view.View" {}

function MenuItem:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menuitem"
end
