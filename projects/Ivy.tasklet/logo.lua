local Color = lost.common.Color

require("sizes")

return dcl.guiro:HBox
{
  bounds = {"left", "top", titleBoxWidth, taskletIconSize}, 
  mode="stack",
  showFrame = false,
  halign="left",
  valign="center",
  dcl.guiro:Image
  {
    bounds = {"left", "top", taskletIconSize, taskletIconSize},
    filename = "leaf.png",
    flip = true,
    filter = true,
    showFrame = false,
    showBackground = false
  },
  dcl.guiro:VBox
  {
    bounds = {"right", "top", titleLabelWidth, taskletIconSize},
    showFrame = false,
    halign="left",
    mode="spread",
    dcl.guiro:Label
    {
      bounds = {"left", "top", titleLabelWidth, 12},
      text = "LostEngine",
      fontSize = 12,
      showShadow = false,
      halign="left",
      textColor = Color(.36, .36, .36)
    },
    dcl.guiro:Label
    {
      bounds = {"left", "bottom", titleLabelWidth, 26},
      text = "Ivy",
      fontSize = 26,
      halign="left",
      showShadow = false,
      halign="left",
      textColor = Color(.6, .6, .6)
    }
  }
}
