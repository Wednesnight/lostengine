local gearsBitmap = lost.bitmap.Bitmap.create(tasklet.loader:load("gears.png"))
gearsBitmap:premultiplyAlpha()

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.Image
    {
      bitmap=gearsBitmap,
      bounds={0,"bottom","1","1"},
      flip=true,
      valign="bottom",
      halign="center"
    },
    lost.guiro.view.ListView
    {
      id="listview",
      alwaysShowCorner = true
    }
  },
  sublayers=
  {
    lost.guiro.layer.Rect
    {
      gradient="candyBlue"
    }
  }
}