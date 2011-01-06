require("lost.guiro.view.Window")

local Color = lost.common.Color

local titleHeight = 30

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  sublayers=
  {
    lost.guiro.layer.Rect{bounds={0,0,"1","1"},color=Color(0,0,0),filled=true}
  },
  subviews=
  {
    lost.guiro.view.Label
    {
      bounds = {0,"top","1",titleHeight},
      text = "Window",
      font = {"Vera",12},
      color = Color(1,1,1)
    },
    lost.guiro.view.Window
    {
      bounds = {20,20,300,200},
      title="Testwindow 1",
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
    },
    lost.guiro.view.Window
    {
      bounds = {20,20,140,300},
      title="Testwindow 2",
      style="normal",
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
  }
}