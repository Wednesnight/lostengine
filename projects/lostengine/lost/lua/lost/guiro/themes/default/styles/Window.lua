-- lost.guiro.themes.default.styles.Window
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.ScrollView")
require("lost.guiro.Bounds")

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

lost.common.Class "lost.guiro.themes.default.styles.Window" "lost.guiro.Style" {}

function Window:constructor()
  lost.guiro.Style(self)
  self.name = "default"
  self.targetClassName = "lost.guiro.Window"
end

function Window:apply(target)
	local c = .27843
	local windowGray = Color(c,c,c,1);
  target:showFrame(true)
  target:showBackground(true)
  target:backgroundColor(windowGray)
  target:frameColor(Color(1,1,1,1))
  
  target:contentView(self:buildScrollView("scrollView"))
end

function Window:buildScrollView(vid)
  local result = lost.guiro.ScrollView()
  result.id = vid
  result.bounds = Bounds(xleft(), ytop(), wrel(1), hrel(1))
  self.theme.themeManager:apply(result, self.theme.name, self.name)
  result:needsLayout()
  return result
end
