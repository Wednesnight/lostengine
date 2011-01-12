return lost.guiro.view.View
{
  subviews =
  {
    lost.guiro.view.TabView
    {
      bounds = {0,0,"1",{"1",-8}},
      style="candyRecess",
      size="regular",
      items={{"Rect",require("layers_rect")},
             {"RoundedRect",require("layers_roundedrect")},
             {"Text",require("layers_text")},
             {"HLine",require("layers_hline")},
             {"Disc",require("layers_disc")},
             {"Image",require("layers_image")}},
    }    
  }
}