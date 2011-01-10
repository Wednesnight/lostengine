require("lost.guiro.layout.Horizontal")
require("lost.guiro.layout.Vertical")
require("lost.guiro.layer.Rect")
require("lost.guiro.view.TabBar")

local Color = lost.common.Color

return lost.guiro.view.View
{
  id="layers_rect",
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="Rectangular area, filled or a frame with variable width, simple color or gradient.",
          color=Color(0,0,0)},
    lost.guiro.layer.Rect{
        bounds={10,10,{"1",-20},{".8",-20}},
        color=Color(.3,.3,.3),
        layout = lost.guiro.layout.Horizontal{},
        sublayers=
        {
          lost.guiro.layer.Layer
          {
            bounds={0,0,".25","1"},
            layout = lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
            sublayers=
            {
              lost.guiro.layer.Rect{bounds={0,0,20,20},color=Color(0,0,0)},
              lost.guiro.layer.Rect{bounds={0,0,50,50},color=Color(1,.2,0)},
              lost.guiro.layer.Rect{bounds={0,0,70,70},color=Color(0,1,.3)},
              lost.guiro.layer.Rect{bounds={0,0,100,80},color=Color(.5,1,.3)},
            }
          },
          lost.guiro.layer.Layer
          {
            bounds={0,0,".25","1"},
            layout = lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
            sublayers=
            {
              lost.guiro.layer.Rect{bounds={0,0,20,20},color=Color(0,0,0),filled=false,width=7},
              lost.guiro.layer.Rect{bounds={0,0,50,50},color=Color(1,.2,0),filled=false,width=5},
              lost.guiro.layer.Rect{bounds={0,0,70,70},color=Color(0,1,.3),filled=false,width=3},
              lost.guiro.layer.Rect{bounds={0,0,100,80},color=Color(.5,1,.3),filled=false,width=1},
            }
          },
          lost.guiro.layer.Layer
          {
            bounds={0,0,".25","1"},
            layout = lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
            sublayers=
            {
              lost.guiro.layer.Rect{bounds={0,0,20,20},color=Color(1,1,1),gradient="rrbg"},
              lost.guiro.layer.Rect{bounds={0,0,50,50},color=Color(1,1,1),gradient="rrbg2"},
              lost.guiro.layer.Rect{bounds={0,0,70,70},color=Color(1,1,1),gradient="candyBlue"},
              lost.guiro.layer.Rect{bounds={0,0,100,80},color=Color(1,1,1),gradient="candyGray"},
            }
          },
          lost.guiro.layer.Layer
          {
            bounds={0,0,".25","1"},
            layout = lost.guiro.layout.Vertical{valign="center", halign="center",spacing=20},
            sublayers=
            {
              lost.guiro.layer.Rect{bounds={0,0,20,20},color=Color(1,1,1),filled=false,width=7, gradient="rrbg"},
              lost.guiro.layer.Rect{bounds={0,0,50,50},color=Color(1,1,1),filled=false,width=5,gradient="rrbg2"},
              lost.guiro.layer.Rect{bounds={0,0,70,70},color=Color(1,1,1),filled=false,width=3,gradient="candyBlue"},
              lost.guiro.layer.Rect{bounds={0,0,100,80},color=Color(1,1,1),filled=false,width=1,gradient="candyGray"},
            }
          },
        }
    }
  }
}