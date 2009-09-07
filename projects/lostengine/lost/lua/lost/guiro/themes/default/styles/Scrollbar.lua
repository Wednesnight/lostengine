module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")

local Color = lost.common.Color

lost.common.Class "lost.guiro.themes.default.styles.Scrollbar" "lost.guiro.Style" {}

function Scrollbar:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.Scrollbar"
end

function Scrollbar:apply(target)
  target:showFrame(true)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  
  target:upButton(self:buildButton("upButton", self.name))
  target:moveButton(self:buildButton("moveButton", "scrollbarHandle"))
  target:downButton(self:buildButton("downButton", self.name))
end

function Scrollbar:buildButton(vid, styleName)
  local result = lost.guiro.Button()
  result.id = vid
  self.theme.themeManager:apply(result, self.theme.name, styleName)
  result:label(lost.guiro.Button.STATE_NORMAL, nil)
  result:label(lost.guiro.Button.STATE_HOVER, nil)
  result:label(lost.guiro.Button.STATE_PUSHED, nil)
  result:label(lost.guiro.Button.STATE_DISABLED, nil)
  result:needsLayout()
  return result
end
