local Color = lost.common.Color
local Vec2 = lost.math.Vec2


return 
lost.guiro.view.View
{
  id="filesEmpty",
--  bounds={fileListWidth,0,{"1",-fileListWidth},"1"},
  subviews=
  {
    lost.guiro.view.Label
    {
      sublayers={lost.guiro.layer.RoundedRect{filled=true,radius=4,color=Color(.6,.8,1)}},
      bounds={"center","center",250,30},
      text="Drop jpg,tga,png or gif files here!",
      color=Color(0,0,0),
      shadow=true,
      shadowColor=Color(1,1,1),
      shadowOffset=Vec2(0,-1),
      halign="center",
    }
  }
}
