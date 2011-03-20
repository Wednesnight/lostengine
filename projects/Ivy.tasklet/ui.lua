return lost.guiro.view.View
{
  listeners=
  {
    drop=function(event)
      mainController:fileDropped(event.filename)
    end
  },
  subviews=
  {
    lost.guiro.view.TabView
    {
      style="square",
      items=
      {
        {"Files",require("ui_files")},
        {"Atlas",require("ui_atlas")},
      }
    }
  }
}