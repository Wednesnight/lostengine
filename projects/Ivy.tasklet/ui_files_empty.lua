local Color = lost.common.Color
local Vec2 = lost.math.Vec2


return 
lost.guiro.view.View
{
  bounds={fileListWidth,0,{"1",-fileListWidth},"1"},
--  sublayers={lost.guiro.layer.RoundedRect{filled=true,radius=4,color=Color(1,1,1)}},
  subviews=
  {
    lost.guiro.view.Label
    {
      bounds={"center","center",100,20},
      text="Drop files here!",
      color=Color(0,0,0),
      shadow=true,
      shadowColor=Color(1,1,1),
      shadowOffset=Vec2(0,-1),
      halign="center",
    }
  }
}
