require("lost.guiro.view.Button")
require("lost.guiro.view.RadioGroup")

local Color = lost.common.Color

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="Button",
          color=Color(0,0,0)},
  },
  subviews=
  {
    lost.guiro.view.Button
    {
      title = "hello",
      bounds = {50,50,100,30}
    },
    lost.guiro.view.Button
    {
      style="checkboxCandy",
      size="mini",
      title = "Checkbox",
      bounds = {50,100,100,30}
    },
    lost.guiro.view.Button
    {
      style="checkboxCandy",
      size="small",
      title = "Checkbox",
      bounds = {50,120,100,30}
    },
    lost.guiro.view.Button
    {
      style="checkboxCandy",
      size="regular",
      title = "Checkbox",
      bounds = {50,140,100,30}
    },
    lost.guiro.view.Button
    {
      style="radioCandy",
      size="regular",
      title = "Radio",
      bounds = {50,"top",100,30}
    },
    lost.guiro.view.Button
    {
      style="radioCandy",
      size="small",
      title = "Radio",
      bounds = {50,{"top",-25},100,30}
    },
    lost.guiro.view.Button
    {
      style="radioCandy",
      size="mini",
      title = "Radio",
      bounds = {50,{"top",-50},100,30}
    },
    lost.guiro.view.RadioGroup
    {
      bounds = {200,200,200,200},
      layout = lost.guiro.layout.Vertical{halign="center",valign="center",spacing = 4},
      subviews = 
      {
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="regular",
          title = "Radio 1",
          bounds = {50,"top",100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="small",
          title = "Radio 2",
          bounds = {50,{"top",-25},100,30}
        },
        lost.guiro.view.Button
        {
          style="radioCandy",
          size="mini",
          title = "Radio 3",
          bounds = {50,{"top",-50},100,30}
        },        
      }
    } 
  }
}