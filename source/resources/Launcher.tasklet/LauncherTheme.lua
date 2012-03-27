lost.common.Class "LauncherTheme" "lost.guiro.Theme" {}

local Color = lost.common.Color
local Bounds = lost.guiro.Bounds

--local rr = lost.guiro.layer.RoundedRect
local rr = lost.guiro.layer.RoundedRect

function LauncherTheme:constructor()
  lost.guiro.Theme.constructor(self)
  self.name = "launcher"
  self:addStyle("lost.guiro.view.Button", "remove", function(target, args) self:removeButton(target, args) end)

  local bmp = lost.bitmap.Bitmap.create(tasklet.loader:load("150.png"))
  bmp:premultiplyAlpha()
  local texparam = lost.gl.Texture.Params()
  self.removeTex = lost.gl.Texture.create(bmp,texparam)
end

function LauncherTheme:removeButton(target,args)
  local l = lost.guiro.layer.Layer
  local i = lost.guiro.layer.Image
  local b = lost.guiro.view.Button
  target:width(16)
  target:height(16)
  local normal = l{sublayers={i{bounds={0,0,"1","1"},texture=self.removeTex}}}
  local pushed = l{sublayers={rr{bounds={0,0,"1","1"},texture=self.removeTex,color=Color(.7,.7,.7)}}}

  target.layer:addSublayer(normal)
  target.layer:addSublayer(pushed)
  target.backgrounds[b.STATE_NORMAL] = normal
  target.backgrounds[b.STATE_PUSHED] = pushed
end
