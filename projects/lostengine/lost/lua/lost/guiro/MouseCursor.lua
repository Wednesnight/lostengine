module("lost.guiro")

require("lost.guiro.View")
require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.MouseCursor" "lost.guiro.View" {}

function MouseCursor:create(properties)
  lost.guiro.View.create(self, properties)
  local g = lost.guiro
  self.bounds = g.Bounds(g.xabs(0), g.yabs(0), g.wabs(self.backgroundImage.width), g.habs(self.backgroundImage.height))
  self:needsLayout()
  self:needsRedraw()
end
