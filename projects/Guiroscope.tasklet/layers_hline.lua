local Color = lost.common.Color

return lost.guiro.view.View
{
  id="layers_hline",
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="A simple horizontal line, one pixel high, simple color, width derived from bounds of layer.",
          color=Color(0,0,0)},
    lost.guiro.layer.Layer
    {
      layout = lost.guiro.layout.Vertical{valign="center", halign="center", spacing="20"},
      sublayers=
      {
        lost.guiro.layer.HLine{bounds={0,0,50,1},color=Color(1,0,0)},
        lost.guiro.layer.HLine{bounds={0,0,70,1},color=Color(1,.3,0)},
        lost.guiro.layer.HLine{bounds={0,0,90,1},color=Color(1,.5,0)},
        lost.guiro.layer.HLine{bounds={0,0,150,1},color=Color(1,.7,.3)},
        lost.guiro.layer.HLine{bounds={0,0,{"1",-150},1},color=Color(0,.7,.3)},
      }
    }
  }
}