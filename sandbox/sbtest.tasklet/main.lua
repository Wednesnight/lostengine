local scrollBarSize = 15
local margin = 0
local sbsize= "regular"

function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      bounds={"center","center",{"1",-margin},{"1",-margin}},
      listeners=
      {
        valueChanged=function(event) 
          log.debug(event.target.id.." "..tostring(event.target:value()))
        end
      },
      subviews=
      {
        lost.guiro.view.ScrollBar
        {
          id="vsb",
          size=sbsize,
          mode="vertical",
          bounds={"right",scrollBarSize,scrollBarSize,{"1",-scrollBarSize}},
        },
        lost.guiro.view.ScrollBar
        {
          id="hsb",
          size=sbsize,
          mode="horizontal",
          bounds={0,0,{"1",-scrollBarSize},scrollBarSize},
        },
        lost.guiro.view.Slider
        {
          mode="horizontal",
          bounds={"center","center",200,20},
          min=0,
          max=1,
          listeners=
          {
            valueChanged = function(event)
              lost.guiro.ui()("view")("vsb"):visibleRange(event.target:value())
              lost.guiro.ui()("view")("hsb"):visibleRange(event.target:value())
            end
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
