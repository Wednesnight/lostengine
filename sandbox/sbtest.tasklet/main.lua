local scrollBarSize = 15
local sbsize= "regular"
cellsize=50
cellspacing=4
gridWidth=4*cellsize+3*cellspacing
gridHeight=5*cellsize+4*cellspacing

local bmp = lost.bitmap.Bitmap.create(tasklet.loader:load("img/cancel.png"))
bmp:premultiplyAlpha()

local Vec2 = lost.math.Vec2

local margin = 0

function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      bounds={margin,margin,{"1",-2*margin},{"1",-2*margin}},
      subviews=
      {
        lost.guiro.view.ScrollView
        {
          size="regular",
          contentSize=Vec2(gridWidth,gridHeight),
          subviews=
          {
            require("grid"),            
          }
        }
      }
    }
  }
end

function update()
--  lost.guiro.ui().layer:print()
--  lost.guiro.ui():printSubviews()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
