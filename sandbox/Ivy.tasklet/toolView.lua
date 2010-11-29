local Color = lost.common.Color

require("sizes")

return dcl.guiro:VBox
{
  bounds = {{"right", -mainBorder}, {"top", -mainBorder}, toolViewWidth, {"1",-mainBorder*2-pathLabelHeight-pathLabelSpacing}},
  showFrame = true,
  halign ="left",
  mode="stack",
  dcl.guiro:Label
  {
    id="sizePixelsLabel",
    bounds = {0,0,"1", 50},
    text="Size (Pixels)"
  },
  dcl.guiro:Label
  {
    id="sizeBytesLabel",
    bounds = {0,0,"1", 50},
    text="Size (Bytes)"
  },
}