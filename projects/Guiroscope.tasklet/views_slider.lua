local Color = lost.common.Color

return lost.guiro.view.View
{
    subviews=
    {
      lost.guiro.view.Label{style="roundFramed",bounds={"center","top",200,30},text="Slider"},
      lost.guiro.view.Slider{bounds={"center","center",200,30}},
    }
}