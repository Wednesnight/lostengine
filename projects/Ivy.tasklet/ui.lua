-- Ivy ui.lua
local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel
local xleft = lost.guiro.xleft
local xright = lost.guiro.xright
local ytop = lost.guiro.ytop
local ybottom = lost.guiro.ybottom
local Color = lost.common.Color

require("sizes")

return dcl.guiro:Screen
{
  dcl.guiro:UserInterface
  {
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      showFrame = false,
      showBackground = false,
      dcl.guiro:View
      {
        bounds = Bounds(xabs(mainBorder), yabs(mainBorder), wrel(1, -mainBorder*2), hrel(1,-mainBorder*2)),
        showFrame = false,
        showBackground = false,
        dcl.guiro:Label
        {
          bounds = Bounds(xabs(10), yabs(10), wabs(50), habs(20)),
          text = "hello"
        },
        require("logo"),
        dcl.guiro:View
        {
          bounds = Bounds(xleft(),ybottom(), wrel(1), hrel(1,-taskletIconSize-3)),
          showFrame = true,
          frameColor = Color(.6, .6, .6)
        }
      }
    }
  }
}