-- Ivy ui.lua
local Bounds = lost.guiro.Bounds
local xcenter = lost.guiro.xcenter
local ycenter = lost.guiro.ycenter
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

local wincol = .1

return dcl.guiro:Screen
{
  dcl.guiro:UserInterface
  {
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      showFrame = false,
      showBackground = true,
      backgroundColor = Color(wincol, wincol, wincol, 1),
      dcl.guiro:View
      {
        bounds = Bounds(xabs(mainBorder), ytop{abs=-mainBorder}, wrel(1, -mainBorder*2-toolViewWidth-toolViewSpacing), hrel(1,-mainBorder*2-pathLabelHeight-pathLabelSpacing)),
        showFrame = false,
        showBackground = false,
        dcl.guiro:Label
        {
          id = "helpLabel",
          bounds = Bounds(xcenter(), ycenter(), wabs(50), habs(20)),
          text = "Drop bitmap here!"
        },
        require("logo"),
        dcl.guiro:View
        {
          bounds = Bounds(xleft(),ybottom(), wrel(1), hrel(1,-taskletIconSize-3)),
          showFrame = true,
          frameColor = Color(.6, .6, .6),
          dcl.guiro:Image
          {
            id = "imageView",
            bounds = Bounds(xcenter(), ycenter(), wrel(1), hrel(1)),
            showFrame = true,
            showBackground = false,
            backgroundColor = Color(1,1,0,1),
            filter = true,
            flip = true
          }
        }
      },
      require("toolView"),
      dcl.guiro:Label
      {
        id = "pathNameLabel",
        bounds = Bounds(xleft{abs=mainBorder}, ybottom{abs=mainBorder}, wrel(1, -mainBorder*2), habs(pathLabelHeight)),
        text = "<no bitmap loaded>",
        showBackground = false,
        showFrame = false
      }      
    }
  }
}