local Color = lost.common.Color

require("lost.guiro.layer.Disc")

return lost.guiro.view.View
{
  id="layers_disc",
  bounds = {0,0,"1","1"},
  clip=true,
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="Disc or ring with variable width, simple color or gradient.",
          color=Color(0,0,0)},
    lost.guiro.layer.Layer
    {
      layout=lost.guiro.layout.Horizontal{spacing=20,valign="center", halign="center"},
      sublayers={
        lost.guiro.layer.Layer
        {
          bounds={0,0,150,"1"},
          layout=lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
          sublayers={
          lost.guiro.layer.Disc{bounds={0,0,10,10},color=Color(1,0,0),filled=true,},
          lost.guiro.layer.Disc{bounds={0,0,20,20},color=Color(1,0,0),filled=true,},
          lost.guiro.layer.Disc{bounds={0,0,50,50},color=Color(1,0,0),filled=true,},
          lost.guiro.layer.Disc{bounds={0,0,100,100},color=Color(1,0,0),filled=true,},
          lost.guiro.layer.Disc{bounds={0,0,150,150},color=Color(1,0,0),filled=true,},
          }
        },
        lost.guiro.layer.Layer
        {
          layout=lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
          bounds={0,0,150,"1"},
          sublayers={
          lost.guiro.layer.Disc{bounds={0,0,10,10},color=Color(1,0,0),filled=false,width=1},
          lost.guiro.layer.Disc{bounds={0,0,20,20},color=Color(1,0,0),filled=false,width=2},
          lost.guiro.layer.Disc{bounds={0,0,50,50},color=Color(1,0,0),filled=false,width=3},
          lost.guiro.layer.Disc{bounds={0,0,100,100},color=Color(1,0,0),filled=false,width=4},
          lost.guiro.layer.Disc{bounds={0,0,150,150},color=Color(1,0,0),filled=false,width=5},
          }
        },
        lost.guiro.layer.Layer
        {
          bounds={0,0,150,"1"},
          layout=lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
          sublayers={
          lost.guiro.layer.Disc{bounds={0,0,10,10},color=Color(1,1,1),filled=true,gradient="candyBlue"},
          lost.guiro.layer.Disc{bounds={0,0,20,20},color=Color(1,1,1),filled=true,gradient="candyBlue"},
          lost.guiro.layer.Disc{bounds={0,0,50,50},color=Color(1,1,1),filled=true,gradient="candyBlue"},
          lost.guiro.layer.Disc{bounds={0,0,100,100},color=Color(1,1,1),filled=true,gradient="candyBlue"},
          lost.guiro.layer.Disc{bounds={0,0,150,150},color=Color(1,1,1),filled=true,gradient="candyBlue"},
          }
        },        
        lost.guiro.layer.Layer
        {
          layout=lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
          bounds={0,0,150,"1"},
          sublayers={
          lost.guiro.layer.Disc{bounds={0,0,10,10},color=Color(1,1,1),filled=false,width=1,gradient="candyBlueFrame"},
          lost.guiro.layer.Disc{bounds={0,0,20,20},color=Color(1,1,1),filled=false,width=2,gradient="candyBlueFrame"},
          lost.guiro.layer.Disc{bounds={0,0,50,50},color=Color(1,1,1),filled=false,width=3,gradient="candyBlueFrame"},
          lost.guiro.layer.Disc{bounds={0,0,100,100},color=Color(1,1,1),filled=false,width=4,gradient="candyBlueFrame"},
          lost.guiro.layer.Disc{bounds={0,0,150,150},color=Color(1,1,1),filled=false,width=5,gradient="candyBlueFrame"},
          }
        },
      }
    }
  }
}