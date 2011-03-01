local scrollBarSize = 15
local margin = 0
local sbsize= "regular"

function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
--    lost.guiro.view.View{style="gray"},
    lost.guiro.view.View
    {
      bounds={"center","center",{"1",-margin},{"1",-margin}},
      subviews=
      {
        lost.guiro.view.ScrollBar
        {
          size=sbsize,
          mode="vertical",
          bounds={"right",scrollBarSize,scrollBarSize,{"1",-scrollBarSize}},
        },
        lost.guiro.view.ScrollBar
        {
          size=sbsize,
          mode="horizontal",
          bounds={0,0,{"1",-scrollBarSize},scrollBarSize},
        },
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
