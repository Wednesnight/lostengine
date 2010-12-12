local tabbarHeight = 30

return lost.guiro.view.View
{
  id="views",
  bounds = {0,0,"1","1"},
  subviews=
  {
    lost.guiro.view.TabBar{
      bounds={"left", "top", "1",tabbarHeight},
      items={"View", "Label","Button", "TabBar", "Image"}
      },
    lost.guiro.view.View
    {
      bounds = {0,0,"1",{"1",-tabbarHeight}},
      layout = lost.guiro.layout.Vertical{halign="center",valign = "center",xoffset = -0,yoffset = 0,spacing = 10,},
      subviews ={
        lost.guiro.view.Label{bounds={200,200,100,30},text="hello",style="roundFramed"},
        lost.guiro.view.Button{id="ugly",mode="sticky",bounds={50,0,100,30},title="Button",},
        lost.guiro.view.Button{id="regular",mode="normal",bounds={50,{"top",-10},154,50},style="rounded",size="regular", title="Regular"},
        lost.guiro.view.Button{id="small",mode="sticky",bounds={50,{"top",-30},154,70},style="rounded",size="small", title="Small"},
        lost.guiro.view.Button{id="mini",bounds={50,{"top",-50},154,90},style="rounded",size="mini", title="Mini"},
        lost.guiro.view.Button{id="regular",mode="toggle",bounds={50,{"top",-10},154,50},style="roundedToggle",size="regular",title="Toggle"},
      }
    }
  }
}
