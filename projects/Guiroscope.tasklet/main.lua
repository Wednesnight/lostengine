--require("lost.common.Profiler")

function focusHandler(event)
  log.debug(event.type.." "..event.target.id)
end

function mouseDownHandler(event)
  log.debug("//////////////////////////////////")
end

function startup()
--  local p = lost.common.Profiler()
--  p:enable()
  local st = lost.platform.currentTimeSeconds()
  require("lost.guiro")

  lost.guiro.ui():add
  {
    lost.guiro.view.View{
      style="gray",
      subviews={
        lost.guiro.view.TabView
        {
          style="square",
          size="regular",
          items={{"Layers",require("layers")},
                 {"Views",require("views")},
                 {"Animations",require("animations")}}
        },
      }
    }
  }

  local et = lost.platform.currentTimeSeconds()
  log.debug("startup in "..tostring(et-st).." seconds")

--  p:disable()
--  p:report()
--  lost.guiro.ui():addEventListener("mouseDown", mouseDownHandler)
--  lost.guiro.ui():addEventListener("focusReceived", focusHandler)
--  lost.guiro.ui():addEventListener("focusLost", focusHandler)
  
--  lost.guiro.ui().layer:print()
--  lost.guiro.ui():printSubviews()
--  lost.guiro.ui().layer.renderNode:print()
end

--function update()
--  log.debug("------------------ UPDATE")
--end

function shutdown()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
