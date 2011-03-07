function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      subviews=
      {
        lost.guiro.view.ListView
        {
          
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
