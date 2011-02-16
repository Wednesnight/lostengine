function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.View{style="gray"},
    lost.guiro.view.ScrollBar
    {
      mode="vertical",
      bounds={50,50,20,200},
    },
    lost.guiro.view.ScrollBar
    {
      mode="horizontal",
      bounds={70,30,200,20},
    }
  }
end

function update()
--  lost.guiro.ui().layer:print()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
