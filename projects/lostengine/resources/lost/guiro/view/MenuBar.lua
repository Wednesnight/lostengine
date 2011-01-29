module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.MenuBar" "lost.guiro.view.View" {}

function MenuBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "menubar"
end
