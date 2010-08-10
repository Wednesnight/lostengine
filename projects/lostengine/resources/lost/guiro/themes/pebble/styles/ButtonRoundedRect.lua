-- lost.guiro.themes.pebble.styles.ButtonRoundedRect
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.view.Image")
require("lost.guiro.view.Label")

local Color = lost.common.Color
local Vec2 = lost.math.Vec2
local Bounds = lost.guiro.Bounds

lost.common.Class "lost.guiro.themes.pebble.styles.ButtonRoundedRect" "lost.guiro.Style" {}

function ButtonRoundedRect:constructor(loader, df, textureManager)
  lost.guiro.Style(self)
  self.textureManager = textureManager
  self.defaultFont = df
  self.name = "RoundedRect"
  self.targetClassName = "lost.guiro.view.Button"
	
	self.fontSize = 16
	self.buttonHeight = 24
	self.cornerRadius = self.buttonHeight / 2
end

function ButtonRoundedRect:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  
  target:background(lost.guiro.view.Button.STATE_NORMAL, self:buildBackgroundImage("normal", "rrbg"))
  target:background(lost.guiro.view.Button.STATE_HOVER, self:buildBackgroundImage("hover", "rrbg"))
  target:background(lost.guiro.view.Button.STATE_PUSHED, self:buildBackgroundImage("pushed", "rrbg2"))
  target:background(lost.guiro.view.Button.STATE_DISABLED, self:buildBackgroundImage("disabled", "rrbg"))
  
  target:label(lost.guiro.view.Button.STATE_NORMAL, self:buildLabel(Color(0,0,0)))
  target:label(lost.guiro.view.Button.STATE_HOVER, self:buildLabel(Color(.5,.5,.5)))
  target:label(lost.guiro.view.Button.STATE_PUSHED, self:buildLabel(Color(0,0,0)))
  target:label(lost.guiro.view.Button.STATE_DISABLED, self:buildLabel(Color(.1,.1,.1)))
  
  target.bounds.height = lost.guiro.hsize{abs=self.buttonHeight}
end

function ButtonRoundedRect:buildBackgroundImage(vid, gradname)
  local result = lost.guiro.view.View(self.textureManager)
  result.id = vid
  result:showFrame(true)
  result:showBackground(true)
	result:backgroundCornerRadius(self.cornerRadius)
	result:backgroundColor(Color(1,1,1))
	result:backgroundGradient(gradname)
	local c = .6588
	result:frameColor(Color(c, c, c))
	result:frameGradient(nil)
	result:frameCornerRadius(9)
	result:backgroundRoundCorners(true, true, true, true)
	result:frameRoundCorners(true, true, true, true)
  result.bounds = Bounds(0, 0, "1", "1")
  result:hidden(true)
  result:needsLayout()
  result:needsRedraw()
  return result
end

function ButtonRoundedRect:buildLabel(col)
  local result = lost.guiro.view.Label(self.textureManager)
  result.bounds = Bounds(0, 0, "1", "1")
  result:font(self.defaultFont)
  result:fontSize(self.fontSize)
  result:text("Button")
  result:textColor(col)
  result:showShadow(false)
--  result:shadowColor(Color(0,0,0,.7))
--  result:shadowOffset(Vec2(1,-1))
  result:halign("center")
  result:valign("center")
  result:showFrame(false)
  result:showBackground(false)
  result:hidden(true)
  result:needsLayout()
  return result  
end