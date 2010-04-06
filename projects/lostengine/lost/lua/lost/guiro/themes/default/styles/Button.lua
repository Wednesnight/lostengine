-- lost.guiro.themes.default.styles.Button
module("lost.guiro.themes.default.styles", package.seeall)

require("lost.common.Class")
require("lost.guiro.Style")
require("lost.guiro.Image")
require("lost.guiro.Bounds")
require("lost.guiro.Label")

local Color = lost.common.Color
local Vec2 = lost.math.Vec2
local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel

lost.common.Class "lost.guiro.themes.default.styles.Button" "lost.guiro.Style" {}

function Button:constructor(loader, df, textureManager)
  lost.guiro.Style(self)
  self.textureManager = textureManager
  self.caps = {left=3, right=3, top=3, bottom=3}
  local texParams = lost.gl.Texture.Params()
  texParams.minFilter = gl.GL_LINEAR
  texParams.magFilter = gl.GL_LINEAR
  self.defaultFont = df
  self.normalTex = lost.gl.Texture.create(loader:load("lost/guiro/themes/default/resources/ButtonNormal.png"), texParams)
  self.hoverTex = lost.gl.Texture.create(loader:load("lost/guiro/themes/default/resources/ButtonHover.png"), texParams)
  self.pushedTex = lost.gl.Texture.create(loader:load("lost/guiro/themes/default/resources/ButtonPushed.png"), texParams)
  self.disabledTex = lost.gl.Texture.create(loader:load("lost/guiro/themes/default/resources/ButtonDisabled.png"), texParams)
  self.name = "default"
  self.targetClassName = "lost.guiro.Button"
end

function Button:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  
  target:background(lost.guiro.Button.STATE_NORMAL, self:buildBackgroundImage(self.normalTex, "normal"))
  target:background(lost.guiro.Button.STATE_HOVER, self:buildBackgroundImage(self.hoverTex, "hover"))
  target:background(lost.guiro.Button.STATE_PUSHED, self:buildBackgroundImage(self.pushedTex, "pushed"))
  target:background(lost.guiro.Button.STATE_DISABLED, self:buildBackgroundImage(self.disabledTex, "disabled"))
  
  target:label(lost.guiro.Button.STATE_NORMAL, self:buildLabel(Color(1,1,1)))
  target:label(lost.guiro.Button.STATE_HOVER, self:buildLabel(Color(1,1,1)))
  target:label(lost.guiro.Button.STATE_PUSHED, self:buildLabel(Color(1,1,1)))
  target:label(lost.guiro.Button.STATE_DISABLED, self:buildLabel(Color(.3,.3,.3)))
end

function Button:buildBackgroundImage(tex, vid)
  local result = lost.guiro.Image(self.textureManager)
  result.id = vid
  result:texture(tex)
  result.bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1))
  result:flip(true)
  result:scale("scalegrid")
  result:caps(self.caps)
  result:showFrame(false)
  result:showBackground(false)
  result:hidden(true)
  result:needsLayout()
  return result
end

function Button:buildLabel(col)
  local result = lost.guiro.Label(self.textureManager)
  result.bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1))
  result:font(self.defaultFont)
  result:fontSize(12)
  result:text("Button")
  result:textColor(col)
  result:showShadow(true)
  result:shadowColor(Color(0,0,0,.7))
  result:shadowOffset(Vec2(1,-1))
  result:halign("center")
  result:valign("center")
  result:showFrame(false)
  result:showBackground(false)
  result:hidden(true)
  result:needsLayout()
  return result  
end