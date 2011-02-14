module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ScrollView" "lost.guiro.view.View" {}

function ScrollView:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "scrollview"
end
