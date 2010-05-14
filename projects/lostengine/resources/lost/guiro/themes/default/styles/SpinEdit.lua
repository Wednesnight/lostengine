module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.Button")
require("lost.guiro.Label")
require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.themes.default.styles.SpinEdit" "lost.guiro.Style" {}

using "lost.guiro.Bounds"

function SpinEdit:constructor(textureManager)
  lost.guiro.Style(self)
  self.textureManager = textureManager
  self.name = "default"
  self.targetClassName = "lost.guiro.SpinEdit"
end

function SpinEdit:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  
  target:label(self:buildLabel("valueLabel", Bounds(0, 0, ".75", "1"), self.name))
  target:upButton(self:buildButton("upButton", Bounds(".75", ".5", ".25", ".5"), "+", self.name))
  target:downButton(self:buildButton("downButton", Bounds(".75", "0", ".25", ".5"), "-", self.name))
end

function SpinEdit:buildButton(vid, bounds, title, styleName)
  local result = lost.guiro.Button(self.textureManager)
  result.id = vid
  result.bounds = bounds
  self.theme.themeManager:apply(result, self.theme.name, styleName)

  for k,state in next,result._states do
    if state.label ~= nil then
      state.label:fontSize(10)
    end
  end
  result:title(title)

  return result
end

function SpinEdit:buildLabel(vid, bounds, styleName)
  local result = lost.guiro.Label(self.textureManager)
  result.id = vid
  result.bounds = bounds
  result:fontSize(10)
  self.theme.themeManager:apply(result, self.theme.name, styleName)
  return result
end
