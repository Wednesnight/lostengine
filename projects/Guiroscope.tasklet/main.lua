
function focusHandler(event)
  log.debug(event.type.." "..event.target.id)
end

function mouseDownHandler(event)
  log.debug("//////////////////////////////////")
end

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
--    lost.guiro.view.FpsMeter
--    {
--      bounds={"right","bottom",140,70}
--    }
  }
  
  lost.guiro.ui():addEventListener("mouseDown", mouseDownHandler)
  lost.guiro.ui():addEventListener("focusReceived", focusHandler)
  lost.guiro.ui():addEventListener("focusLost", focusHandler)
  
--  lost.guiro.ui().layer:print()
--  lost.guiro.ui():printSubviews()
--  lost.guiro.ui().layer.renderNode:print()
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
