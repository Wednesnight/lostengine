function sv(containerBounds,sbsize,scrollbarSizePixel)
  return lost.guiro.view.View
  {
    bounds=containerBounds,
    subviews=
    {
      lost.guiro.view.ScrollBar
      {
        id="vsb",
        size=sbsize,
        mode="vertical",
        bounds={"right",scrollbarSizePixel,scrollbarSizePixel,{"1",-scrollbarSizePixel}},
      },
      lost.guiro.view.ScrollBar
      {
        id="hsb",
        size=sbsize,
        mode="horizontal",
        bounds={0,0,{"1",-scrollbarSizePixel},scrollbarSizePixel},
      },
    }
  }
end


local vs = 200
local offset = 10
return lost.guiro.view.View
{
  subviews=
  {
    sv({"left","top",{".3",-offset},"1"},"regular",16),
    sv({"center","top",{".3",-offset},"1"},"small",12),
    sv({"right","top",{".3",-offset},"1"},"mini",10),
  }
}