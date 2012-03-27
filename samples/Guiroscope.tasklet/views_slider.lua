local Color = lost.common.Color

return lost.guiro.view.View
{
    listeners =
    {
      valueChanged = function(event) 
        event.currentTarget("val"):text(string.format("%1.1f",event.target:value()))
      end
    },
    subviews=
    {
      lost.guiro.view.Label{id="val",bounds={"center",{"top",-100},200,20},text="slider value"},
      lost.guiro.view.Label{style="roundFramed",bounds={"center","top",200,30},text="Slider"},
      lost.guiro.view.Slider{bounds={50,100,200,30},size="regular",mode="horizontal",min=3,max=5},
      lost.guiro.view.Slider{bounds={50,150,200,30},size="small",mode="horizontal",min=3,max=5},
      lost.guiro.view.Slider{bounds={50,200,200,30},size="mini",mode="horizontal",min=3,max=5},

      lost.guiro.view.Slider{bounds={300,100,30,200},size="regular",mode="vertical",min=3,max=5},
      lost.guiro.view.Slider{bounds={350,100,30,200},size="small",mode="vertical",min=3,max=5},
      lost.guiro.view.Slider{bounds={400,100,30,200},size="mini",mode="vertical",min=3,max=5},
    }
}