require("lost.guiro.view.Box")

local Color = lost.common.Color

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="View",
          color=Color(0,0,0)},
  },
  subviews=
  {
    lost.guiro.view.Box
    {
      bounds={20,20,200,200},
      title="Box",
      subviews=
      {
        lost.guiro.view.Button
        {
          bounds={0,"top","1",30},
          title="Testing",
        }
      }
    }
  }
}