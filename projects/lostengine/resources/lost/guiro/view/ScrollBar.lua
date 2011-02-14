module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollBar" "lost.guiro.view.View" {}

function ScrollBar:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollbar"
end
