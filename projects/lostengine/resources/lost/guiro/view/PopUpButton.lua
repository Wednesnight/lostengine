module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.PopUpButton" "lost.guiro.view.Button" {}

function PopUpButton:constructor(args)
  lost.guiro.view.Button.constructor(self, args)
  local t = args or {}
  self.id = t.id or "popupbutton"
end
