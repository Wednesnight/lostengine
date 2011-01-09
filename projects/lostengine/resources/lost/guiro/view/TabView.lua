module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.TabView" "lost.guiro.view.View" {}

function TabView:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
end

