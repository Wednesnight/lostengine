local Color = lost.common.Color

return lost.guiro.view.View
{
  id="layers_roundedrect",
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="Rounded Rect.",
          color=Color(0,0,0)},
  }
}