
function startup()
  require("lost.guiro")
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  lost.guiro.ui():add
  {
    lost.guiro.view.TabView
    {
      style="square",
      size="regular",
      items={{"Layers",require("layers")},
             {"Views",require("views")}}
    }
  }
--  ui.layer:print()
--  ui:printSubviews()
--  ui.layer.renderNode:print()
end

function update()
end

function shutdown()
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
