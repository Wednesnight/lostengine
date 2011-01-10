
local Color = lost.common.Color

function startup()
  require("lost.guiro")
  local tabbarheight = 52
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)


  ui = lost.guiro.view.UserInterface
  {
    subviews = 
    {
      lost.guiro.view.TabView
      {
        size="regular",
        style="square",
        items={{"Layers",require("layers")},
               {"Views",require("views")}}
      }
    }
  }
--  ui.layer:print()
--  ui:printSubviews()
end

function update()
--  ui.layer.renderNode:print()
--  local u = gcinfo();collectgarbage("collect");local u2 = gcinfo();log.debug("used: "..u.."kb after gc: "..u2.."kb")
end

function shutdown()
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
