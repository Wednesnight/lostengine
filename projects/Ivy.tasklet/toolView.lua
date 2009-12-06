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

return dcl.guiro:VBox
{
  bounds = Bounds(xright{abs=-mainBorder}, ytop{abs=-mainBorder},wabs(toolViewWidth), hrel(1,-mainBorder*2-pathLabelHeight-pathLabelSpacing)),
  showFrame = true,
  halign ="left",
  mode="stack",
  dcl.guiro:Label
  {
    id="sizePixelsLabel",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(50)),
    text="Size (Pixels)"
  },
  dcl.guiro:Label
  {
    id="sizeBytesLabel",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(50)),
    text="Size (Bytes)"
  },
}