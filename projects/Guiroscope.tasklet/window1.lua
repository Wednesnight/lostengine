return 
lost.guiro.view.Window
{
  bounds = {20,20,300,200},
  title="Window 1",
  id="window1",
  closeButton = true,
  subviews =
  {
    lost.guiro.view.RadioGroup
    {
      bounds = {0,0,"1","1"},
      layout = lost.guiro.layout.Vertical{halign="center",valign="center",halign="center",spacing = 4},
      subviews = 
      {
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 1",
          id="rg1",
          bounds = {0,0,80,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 2",
          id="rg2",
          bounds = {0,0,80,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 3",
          id="rg3",
          bounds = {0,0,80,30}
        },        
      }
    }
  } 
}
