return lost.guiro.view.View
{
  subviews=
  {
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