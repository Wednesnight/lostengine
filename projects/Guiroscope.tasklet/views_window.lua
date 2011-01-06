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
      movable=true,
      moveType="body"
    }
  }
}