module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")
require("lost.guiro.Singletons")

lost.common.Class "lost.guiro.view.Label" "lost.guiro.view.View" {}

function Label:constructor(args)
  lost.guiro.view.View.constructor(self, args)
  local t = args or {}
  self.id = t.id or "label"
  self.textLayer = self.layer:recursiveFindById("text")
  if t.font then
    self.textLayer:font(tasklet.fontManager:getFont(t.font[1], t.font[2]))
  end
  if t.text then
    self.textLayer:text(t.text)
  end
  if t.color then
    self.textLayer:color(t.color)
  end
  if t.valign then
    self.textLayer:valign(t.valign)
  end
  if t.halign then
    self.textLayer:halign(t.halign)
  end
end



