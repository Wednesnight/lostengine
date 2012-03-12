local Color = lost.common.Color

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end

local m = 10

function startup()
  require("lost/guiro")

  local fnt = tasklet.fontManager:getFont("Vera", 12)

  lost.guiro.ui():add{
    lost.guiro.view.View
    {
      style="gray",
      subviews={
        lost.guiro.view.Label
        {
          bounds={m,{"top",-m},200,2*m},
          font={"Vera", 12},
          text = "Hello",
          halign="right",
          sublayers = {lost.guiro.layer.Rect{color=Color(1,0,0)}}
        },
      }
    }
  }
end