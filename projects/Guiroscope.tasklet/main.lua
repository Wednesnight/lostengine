
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
    },
    lost.guiro.view.FpsMeter
    {
      bounds={"right","bottom",140,70}
    }
  }
--  ui.layer:print()
--  ui:printSubviews()
--  ui.layer.renderNode:print()
end

--function update()
--  log.debug("------------------ UPDATE")
--end

function shutdown()
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
