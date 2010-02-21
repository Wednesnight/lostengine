module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.Button")
require("lost.guiro.Label")
require("lost.guiro.Bounds")

lost.common.Class "lost.guiro.themes.default.styles.SpinEdit" "lost.guiro.Style" {}

using "lost.guiro.Bounds"
using "lost.guiro.xrel"
using "lost.guiro.yrel"
using "lost.guiro.wrel"
using "lost.guiro.hrel"

function SpinEdit:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.SpinEdit"
end

function SpinEdit:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  
  target:downButton(self:buildButton("downButton", Bounds(xrel(0), yrel(0), wrel(.25), hrel(1)), "-", self.name))
  target:label(self:buildLabel("valueLabel", Bounds(xrel(.25), yrel(0), wrel(.5), hrel(1)), self.name))
  target:upButton(self:buildButton("upButton", Bounds(xrel(.75), yrel(0), wrel(.25), hrel(1)), "+", self.name))
end

function SpinEdit:buildButton(vid, bounds, title, styleName)
  local result = lost.guiro.Button()
  result.id = vid
  result.bounds = bounds
  self.theme.themeManager:apply(result, self.theme.name, styleName)
  result:title(title)
  return result
end

function SpinEdit:buildLabel(vid, bounds, styleName)
  local result = lost.guiro.Label()
  result.id = vid
  result.bounds = bounds
  self.theme.themeManager:apply(result, self.theme.name, styleName)
  return result
end
