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

return dcl.guiro:HBox
{
  bounds = Bounds(xleft(), ytop(), wabs(titleBoxWidth), habs(taskletIconSize)),
  mode="stack",
  showFrame = false,
  halign="left",
  valign="center",
  dcl.guiro:Image
  {
    bounds = Bounds(xleft(), ytop(), wabs(taskletIconSize), habs(taskletIconSize)),
    filename = "leaf.png",
    flip = true,
    filter = true,
    showFrame = false,
    showBackground = false
  },
  dcl.guiro:VBox
  {
    bounds = Bounds(xright(), ytop(), wabs(titleLabelWidth), habs(taskletIconSize)),
    showFrame = false,
    halign="left",
    mode="spread",
    dcl.guiro:Label
    {
      bounds = Bounds(xleft(), ytop(), wabs(titleLabelWidth), habs(12)),
      text = "LostEngine",
      fontSize = 12,
      showShadow = false,
      halign="left",
      textColor = Color(.36, .36, .36)
    },
    dcl.guiro:Label
    {
      bounds = Bounds(xleft(), ybottom(), wabs(titleLabelWidth), habs(26)),
      text = "Ivy",
      fontSize = 26,
      halign="left",
      showShadow = false,
      halign="left",
      textColor = Color(.6, .6, .6)
    }
  },
}
