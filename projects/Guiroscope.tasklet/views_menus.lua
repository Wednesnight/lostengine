return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.MenuBar
    {
      bounds={0,"top","1",30},
      items={
        {"Menu 1", 
          {
            {title="m1 First"},
            {title="m1 Second"},
            "-",
            {title="m1 Third"},              
            {title="m1 Fourth"},              
          }
        },
        {"Menu 2", 
          {
            {title="m2 First"},
            {title="m2 Second"},
            "-",
            {title="m2 Third"},              
            {title="m2 Fourth"},              
            "-",
            {title="m2 Fifth"},              
          }
        }
      }
    },
    lost.guiro.view.MenuBarItem
    {
      bounds = {50,50,200,22},
      title = "Hello",
    }
  }
}