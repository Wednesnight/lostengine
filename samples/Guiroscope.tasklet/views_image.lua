local Color = lost.common.Color

local bmp = lost.bitmap.Bitmap.create(tasklet.loader:load("test.png"))
bmp:premultiplyAlpha()

return lost.guiro.view.View
{
  bounds = {0,0,"1","1"},
  sublayers =
  {
    lost.guiro.layer.Text{bounds={"center", "top",200,20},font={"Vera",12},
          text="Image",
          color=Color(0,0,0)},
  },
  subviews=
  {
    lost.guiro.view.Image
    {
      bounds={0,0,500,500},
      bitmap=bmp,
      scale="none",
      flip=true,
      filter=false,
    }
  }
}