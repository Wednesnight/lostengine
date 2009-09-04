module("lost.guiro", package.seeall)

require("lost.guiro.View")
require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.Window" "lost.guiro.View" {}

function Window:constructor()
  lost.guiro.View.constructor(self)
end

function Window:contentView(view)
  if view ~= nil then
    self._contentView = view
    lost.guiro.View.addSubview(self, view)
  else
    return self._contentView
  end
end

function Window:addSubview(newview, pos)
  self._contentView:addSubview(newview, pos)
end
