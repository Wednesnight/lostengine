-- lost.guiro.themes.default.styles.ScrollbarHandle
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

lost.common.Class "lost.guiro.themes.default.styles.ScrollbarHandle" "lost.guiro.Style" {}

function ScrollbarHandle:constructor(loader)
  lost.guiro.Style(self)
  self.caps = {left=3, right=3, top=3, bottom=3}
  local texParams = lost.gl.Texture.Params()
  texParams.minFilter = gl.GL_LINEAR
  texParams.magFilter = gl.GL_LINEAR
  self.normalTex = lost.gl.Texture(loader:load("lost/guiro/themes/default/resources/ButtonNormal.png"), texParams)
  self.hoverTex = lost.gl.Texture(loader:load("lost/guiro/themes/default/resources/ButtonHover.png"), texParams)
  self.pushedTex = lost.gl.Texture(loader:load("lost/guiro/themes/default/resources/ButtonPushed.png"), texParams)
  self.disabledTex = lost.gl.Texture(loader:load("lost/guiro/themes/default/resources/ButtonDisabled.png"), texParams)
  self.name = "scrollbarHandle"
  self.targetClassName = "lost.guiro.Button"
end

function ScrollbarHandle:apply(target)
  target:showFrame(false)
  target:showBackground(false)
  target:backgroundColor(Color(.5,.5,.5,1))
  target:frameColor(Color(1,1,1,1))
  
  target:background(lost.guiro.Button.STATE_NORMAL, self:buildBackgroundImage(self.normalTex, "normal", Color(.8,.8,.8,1)))
  target:background(lost.guiro.Button.STATE_HOVER, self:buildBackgroundImage(self.hoverTex, "hover", Color(1,1,1,1)))
  target:background(lost.guiro.Button.STATE_PUSHED, self:buildBackgroundImage(self.pushedTex, "pushed", Color(.4,.4,.4,1)))
  target:background(lost.guiro.Button.STATE_DISABLED, self:buildBackgroundImage(self.disabledTex, "disabled", Color(.6,.6,.6,1)))
  
  target:label(lost.guiro.Button.STATE_NORMAL, nil)
  target:label(lost.guiro.Button.STATE_HOVER, nil)
  target:label(lost.guiro.Button.STATE_PUSHED, nil)
  target:label(lost.guiro.Button.STATE_DISABLED, nil)
end

function ScrollbarHandle:buildBackgroundImage(tex, vid, col)
  local result = lost.guiro.Image()
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
  result:color(col)
  return result
end
