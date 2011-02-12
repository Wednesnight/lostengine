local Color = lost.common.Color

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.PopUpButton
    {
      listeners=
      {
        menuItemSelected=function(event) log.debug("mi selected") end
      },
      bounds={"center","center",100,30},
      title = "Colors",
      menu={
        items=
        {
          {title="m1 First"},
          {title="m1 Second"},
          "-",
          {title="m1 Third"},              
          {title="m1 Fourth"},              
        }
      },
    }
  }
}