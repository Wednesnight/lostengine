module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.PopUpButton" "lost.guiro.view.View" {}

function PopUpButton:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "popupbutton"
end
