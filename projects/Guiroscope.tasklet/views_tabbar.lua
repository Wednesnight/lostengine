local Color = lost.common.Color

local tabbarHeight = 20

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,tabbarHeight},font={"Vera",12},
          text="TabBar",
          color=Color(0,0,0)},
  },
  subviews =
  {
    lost.guiro.view.View
    {
      bounds={0,0,300,{"1",-tabbarHeight}},
      layout = lost.guiro.layout.Vertical{valign="center",halign="center",spacing=20},
      subviews=
      {
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Regular size","Item 1", "Item 2", "Item 3"},size="regular"},
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Small size","Item 1", "Item 2", "Item 3"},size="small"},
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Mini size","Item 1", "Item 2", "Item 3"},size="mini"},
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Single item"}},
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Two items", "Another one"}},
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Three items", "Another one", "Yay, more!"}},
        lost.guiro.view.TabBar{bounds={0,0,300,30},items={"Four items", "Another one", "Yay, more!", "W00t?"}, clip=true},
      }
    },    
    lost.guiro.view.View
    {
      bounds={300,0,300,{"1",-tabbarHeight}},
      layout = lost.guiro.layout.Vertical{valign="center",halign="center",spacing=20},
      subviews=
      {
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Regular size","Item 1", "Item 2", "Item 3"},size="regular"},
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Small size","Item 1", "Item 2", "Item 3"},size="small"},
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Mini size","Item 1", "Item 2", "Item 3"},size="mini"},
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Single item"}},
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Two items", "Another one"}},
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Three items", "Another one", "Yay, more!"}},
        lost.guiro.view.TabBar{bounds={0,0,300,30},style="square",items={"Four items", "Another one", "Yay, more!", "W00t?"}, clip=true},
      }
    }    
  }
}