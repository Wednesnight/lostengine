module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.Scrollbar")

local Color = lost.common.Color
local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
local xright = lost.guiro.xright
local xleft = lost.guiro.xleft
local ytop = lost.guiro.ytop
local ybottom = lost.guiro.ybottom
local xcenter = lost.guiro.xcenter
local ycenter = lost.guiro.ycenter
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

lost.common.Class "lost.guiro.themes.default.styles.ScrollView" "lost.guiro.Style" {}

function ScrollView:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.ScrollView"
end

function ScrollView:apply(target)
  target:showFrame(true)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  
  target:contentView(self:buildContentView("contentView"))
  target:horizontalScrollbar(self:buildScrollbar("horizontalScrollbar", "horizontal"))
  target:verticalScrollbar(self:buildScrollbar("verticalScrollbar", "vertical"))
end

function ScrollView:buildContentView(vid)
  local result = lost.guiro.View()
  result.id = vid
  self.theme.themeManager:apply(result, self.theme.name, self.name)
  result:showFrame(false)
  result:showBackground(false)
  -- FIXME: remove when we have FloatView
  result.bounds = Bounds(xleft(), ytop(), wabs(2000), habs(2000))
  return result
end

function ScrollView:buildScrollbar(vid, orientation)
  local result = lost.guiro.Scrollbar()
  result.id = vid
  self.theme.themeManager:apply(result, self.theme.name, self.name)
  result:orientation(orientation)
  result:needsLayout()
  return result
end
