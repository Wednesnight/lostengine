local Color = lost.common.Color

require("lost.guiro.layer.Disc")

return lost.guiro.view.View
{
  id="layers_disc",
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="Disc or ring with variable width, simple color or gradient.",
          color=Color(0,0,0)},
    lost.guiro.layer.Disc{bounds={50,50,200,200},color=Color(1,0,0),filled=true,width=4}
  }
}