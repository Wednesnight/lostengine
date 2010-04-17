using "lost.application.WindowParams"
using "lost.math.Rect"

local _meta =
{
  name = "Sh4d0w",
  windowRect = Rect(100,100,640,480)
}

windowParams = WindowParams(_meta.name, _meta.windowRect)
return _meta
