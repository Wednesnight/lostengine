module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.ListView" "lost.guiro.view.ScrollView" {}

local Color = lost.common.Color
local Vec2 = lost.math.Vec2

function ListView:constructor(args)
  lost.guiro.view.ScrollView.constructor(self, args)
  local t = args or {}
  self.id = t.id or "listview"
  self:hasHorizontalScrollbar(false)
  self:hasVerticalScrollbar(true)  
end