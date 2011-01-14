local Color = lost.common.Color

return lost.guiro.view.View
{
    subviews=
    {
      lost.guiro.view.Label{style="roundFramed",bounds={"center","top",200,30},text="Slider"},
      lost.guiro.view.Slider{bounds={50,"center",200,30},min=3,max=5},
      lost.guiro.view.Slider{bounds={300,"center",30,200},mode="vertical",min=3,max=5},
    }
}