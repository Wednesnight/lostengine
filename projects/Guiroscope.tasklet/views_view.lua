require("lost.guiro.view.Box")
require("lost.guiro.view.TabView")

local Color = lost.common.Color

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="View",
          color=Color(0,0,0)},
  },
  subviews=
  {
    lost.guiro.view.Box
    {
      bounds={20,20,200,200},
      title="Box",
      subviews=
      {
        lost.guiro.view.Button
        {
          bounds={0,"top","1",30},
          title="Testing",
        },        
      }
    },
    lost.guiro.view.TabView
    {
      style="candyRoundedRecess",
      bounds={250,20,300,300},
      items={{"view1",lost.guiro.view.Button{title="Hello 1"}},
             {"view2",lost.guiro.view.Button{title="Hello 2",bounds={0,"top",50,50}}},
             {"view3",lost.guiro.view.Button{title="Hello 3",bounds={"right","center",80,80}}}
             }
    },
    lost.guiro.view.TabView
    {
      style="candyRecess",
      bounds={250,350,300,300},
      items={{"view1",lost.guiro.view.Button{title="Hello 1"}},
             {"view2",lost.guiro.view.Button{title="Hello 2",bounds={0,"top",50,50}}},
             {"view3",lost.guiro.view.Button{title="Hello 3",bounds={"right","center",80,80}}}
             }
    },
    lost.guiro.view.TabView
    {
      style="square",
      bounds={650,20,200,300},
      items={{"view1",lost.guiro.view.Button{title="Hello 1"}},
             {"view2",lost.guiro.view.Button{title="Hello 2",bounds={0,"top",50,50}}},
             {"view3",lost.guiro.view.Button{title="Hello 3",bounds={"right","center",80,80}}}
             }
    }    


  }
}