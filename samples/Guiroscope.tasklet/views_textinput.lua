local w1 = require("focustest_window")

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.TextInput
    {
      bounds={50,{"top",-50},100,30},
      id="ti1",
    },
    lost.guiro.view.TextInput
    {
      bounds={250,{"top",-50},100,30},
      id="ti2",
    },
    lost.guiro.view.Button{
        bounds={"center","center",200,20},
        size="regular",
        title="Click to open Window",
        listeners={buttonClick=function(event) w1:open() end}
    }
    
  }
}