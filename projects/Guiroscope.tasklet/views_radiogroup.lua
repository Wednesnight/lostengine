return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.RadioGroup
    {
      bounds = {200,200,200,200},
      layout = lost.guiro.layout.Vertical{halign="center",valign="center",spacing = 4},
      subviews = 
      {
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="mini",
          title = "Radio 1",
          id="rg1",
          bounds = {50,"top",100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="mini",
          title = "Radio 2",
          id="rg2",
          bounds = {50,{"top",-25},100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="mini",
          title = "Radio 3",
          id="rg3",
          bounds = {50,{"top",-50},100,30}
        },        
      }
    },
    lost.guiro.view.RadioGroup
    {
      bounds = {400,200,200,200},
      layout = lost.guiro.layout.Vertical{halign="center",valign="center",spacing = 4},
      subviews = 
      {
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="small",
          title = "Radio 1",
          id="rg1",
          bounds = {50,"top",100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="small",
          title = "Radio 2",
          id="rg2",
          bounds = {50,{"top",-25},100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="small",
          title = "Radio 3",
          id="rg3",
          bounds = {50,{"top",-50},100,30}
        },        
      }
    },
    lost.guiro.view.RadioGroup
    {
      bounds = {600,200,200,200},
      layout = lost.guiro.layout.Vertical{halign="center",valign="center",spacing = 4},
      subviews = 
      {
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 1",
          id="rg1",
          bounds = {50,"top",100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 2",
          id="rg2",
          bounds = {50,{"top",-25},100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 3",
          id="rg3",
          bounds = {50,{"top",-50},100,30}
        },        
      }
    } 
  }
}