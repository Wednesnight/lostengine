local Color = lost.common.Color

local descriptionHeight = 20

return lost.guiro.view.View
{
  id="layers_roundedrect",
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,descriptionHeight},font={"Vera",12},
          text="Rounded rectangle, filled or as frame, simple color or gradient, whose corners can be switched to non-rounded and sides can be disabled if it is used as frame.",
          color=Color(0,0,0)},
    lost.guiro.layer.Layer
    {
      bounds={0,0,"1",{"1",-descriptionHeight}},
      layout=lost.guiro.layout.Vertical{valign="center", halign="center", spacing=10},
      sublayers=
      {
        lost.guiro.layer.Layer{
          id="filled rrs",
          bounds={0,0,"1",50},
          layout = lost.guiro.layout.Horizontal{valign="center", halign="center", spacing=10},
          sublayers=
          {
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(0,0,0),roundCorners={tl=true,tr=true,bl=true,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.1,0,0),roundCorners={tl=false,tr=true,bl=true,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.2,0,0),roundCorners={tl=true,tr=false,bl=true,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.3,0,0),roundCorners={tl=true,tr=true,bl=false,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.4,0,0),roundCorners={tl=false,tr=true,bl=true,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.5,0,0),roundCorners={tl=false,tr=false,bl=true,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.6,0,0),roundCorners={tl=true,tr=false,bl=false,br=true}},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=true, color=Color(.7,0,0),roundCorners={tl=true,tr=true,bl=false,br=false}},
          }
        },
        lost.guiro.layer.Layer{
          id="filled rrs",
          bounds={0,0,"1",100},
          layout = lost.guiro.layout.Horizontal{valign="center", halign="center", spacing=10},
          sublayers=
          {
            lost.guiro.layer.RoundedRect{bounds={0,0,60,60},radius=8, filled=false, color=Color(0,0,0),roundCorners={tl=true,tr=true,bl=true,br=true},width=10},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,65},radius=8, filled=false, color=Color(0,.1,0),roundCorners={tl=false,tr=true,bl=true,br=true},width=9},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,70},radius=8, filled=false, color=Color(0,.2,0),roundCorners={tl=true,tr=false,bl=true,br=true},width=8},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,75},radius=8, filled=false, color=Color(0,.3,0),roundCorners={tl=true,tr=true,bl=false,br=true},width=7},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,80},radius=8, filled=false, color=Color(0,.4,0),roundCorners={tl=false,tr=true,bl=true,br=true},width=6},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,85},radius=8, filled=false, color=Color(0,.5,0),roundCorners={tl=false,tr=false,bl=true,br=true},width=5},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,90},radius=8, filled=false, color=Color(0,.6,0),roundCorners={tl=true,tr=false,bl=false,br=true},width=4},
            lost.guiro.layer.RoundedRect{bounds={0,0,60,95},radius=8, filled=false, color=Color(0,.7,0),roundCorners={tl=true,tr=true,bl=false,br=false},width=3},
          }
        },
        lost.guiro.layer.Layer{
          id="filled rrs",
          bounds={0,0,"1",50},
          layout = lost.guiro.layout.Horizontal{valign="center", halign="center", spacing=10},
          sublayers=
          {
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,0),roundCorners={tl=true,tr=true,bl=true,br=true},sides={top=true,bottom=true,left=true,right=true},width=1},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.1),roundCorners={tl=false,tr=true,bl=true,br=true},sides={top=false,bottom=true,left=true,right=true},width=2},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.2),roundCorners={tl=true,tr=false,bl=true,br=true},sides={top=true,bottom=false,left=true,right=true},width=3},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.3),roundCorners={tl=true,tr=true,bl=false,br=true},sides={top=true,bottom=true,left=false,right=true},width=4},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.4),roundCorners={tl=false,tr=true,bl=true,br=true},sides={top=true,bottom=true,left=true,right=false},width=5},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.5),roundCorners={tl=false,tr=false,bl=true,br=true},sides={top=false,bottom=false,left=true,right=true},width=6},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.6),roundCorners={tl=true,tr=true,bl=false,br=true},sides={top=true,bottom=false,left=false,right=true},width=7},
            lost.guiro.layer.RoundedRect{bounds={0,0,40,40},radius=8, filled=false, color=Color(0,0,.7),roundCorners={tl=true,tr=true,bl=false,br=false},sides={top=true,bottom=true,left=false,right=false},width=8},
          }
        },
        lost.guiro.layer.Layer{
          id="filled rrs",
          bounds={0,0,"1",50},
          sublayers=
          {
            lost.guiro.layer.RoundedRect{bounds={10,10,{"1",-20},"1"},radius=10, filled=true, color=Color(1,1,1),gradient="candyBlue",roundCorners={tl=true,tr=true,bl=true,br=true},sides={top=true,bottom=true,left=true,right=true},width=4,gradientVertical=false},            
            lost.guiro.layer.RoundedRect{bounds={10,10,{"1",-20},"1"},radius=10, filled=false, color=Color(1,1,1),gradient="candyBlueFrame",roundCorners={tl=true,tr=true,bl=true,br=true},sides={top=true,bottom=true,left=true,right=true},width=4,gradientVertical=false},            
          }
        },
        lost.guiro.layer.Layer{
          id="filled rrs",
          bounds={0,0,"1",50},
          sublayers=
          {
            lost.guiro.layer.RoundedRect{bounds={10,10,400,50},radius=10, filled=true, color=Color(1,1,1),gradient="candyBlue",roundCorners={tl=true,tr=false,bl=true,br=false},sides={top=true,bottom=true,left=true,right=true},width=4},            
            lost.guiro.layer.RoundedRect{bounds={10,10,400,50},radius=10, filled=false, color=Color(1,1,1),gradient="candyBlueFrame",roundCorners={tl=true,tr=false,bl=true,br=false},sides={top=true,bottom=true,left=true,right=true},width=4},            
          }
        }
      }
    }
  }
}