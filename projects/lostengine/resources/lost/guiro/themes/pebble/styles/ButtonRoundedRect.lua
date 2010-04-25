-- lost.guiro.themes.pebble.styles.ButtonRoundedRect
module("lost.guiro.themes.pebble.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.Image")
require("lost.guiro.Bounds")
require("lost.guiro.Label")
require("lost.guiro.Bounds")

local Color = lost.common.Color
local Vec2 = lost.math.Vec2
local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

lost.common.Class "lost.guiro.themes.pebble.styles.ButtonRoundedRect" "lost.guiro.Style" {}

function ButtonRoundedRect:constructor(loader, df, textureManager)
  lost.guiro.Style(self)
  self.textureManager = textureManager
  self.defaultFont = df
  self.name = "RoundedRect"
  self.targetClassName = "lost.guiro.Button"
	
	self.fontSize = 16
	self.buttonHeight = 24
	self.cornerRadius = self.buttonHeight / 2
end

function ButtonRoundedRect:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  
  target:background(lost.guiro.Button.STATE_NORMAL, self:buildBackgroundImage("normal", "rrbg"))
  target:background(lost.guiro.Button.STATE_HOVER, self:buildBackgroundImage("hover", "rrbg"))
  target:background(lost.guiro.Button.STATE_PUSHED, self:buildBackgroundImage("pushed", "rrbg2"))
  target:background(lost.guiro.Button.STATE_DISABLED, self:buildBackgroundImage("disabled", "rrbg"))
  
  target:label(lost.guiro.Button.STATE_NORMAL, self:buildLabel(Color(0,0,0)))
  target:label(lost.guiro.Button.STATE_HOVER, self:buildLabel(Color(.2,.2,.2)))
  target:label(lost.guiro.Button.STATE_PUSHED, self:buildLabel(Color(0,0,0)))
  target:label(lost.guiro.Button.STATE_DISABLED, self:buildLabel(Color(.1,.1,.1)))
  
  target.bounds.height = lost.guiro.habs(self.buttonHeight)
end

function ButtonRoundedRect:buildBackgroundImage(vid, gradname)
  local result = lost.guiro.View(self.textureManager)
  result.id = vid
  result:showFrame(true)
  result:showBackground(true)
	result:backgroundCornerRadius(self.cornerRadius)
	result:backgroundColor(Color(1,1,1))
	result:backgroundGradient(self.textureManager._textureManager.gradientTexture, self.textureManager:gradientCoord(gradname))
	local c = .6588
	result:frameColor(Color(c, c, c))
	result:frameCornerRadius(9)
	result:backgroundRoundCorners(true, true, true, true)
	result:frameRoundCorners(true, true, true, true)
  result.bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1))
  result:hidden(true)
  result:needsLayout()
  result:needsRedraw()
  return result
end

function ButtonRoundedRect:buildLabel(col)
  local result = lost.guiro.Label(self.textureManager)
  result.bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1))
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