require("lost.guiro.Bounds")

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.guiro.Bounds"

using "lost.guiro.xleft"
using "lost.guiro.yabs"
using "lost.guiro.wabs"
using "lost.guiro.habs"

screensize = Vec2(320,480)
windowParams = WindowParams("LayoutIntegration", Rect(0,0,screensize.x, screensize.y))
running = true
function startup(tasklet)
  tasklet.name = "LayoutIntegration"
  tasklet.waitForEvents = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  log.debug("----------------------------------------------")
  log.debug("----------------------------------------------")
  local b1 = Bounds(xleft(),yabs(59), wabs(20), habs(20))
  log.debug("b1 complete: "..tostring(b1:complete()))
  local b2 = Bounds(13,13, 13, 13)
  log.debug("b2 complete: "..tostring(b2:complete()))
  return running
end

function update(tasklet)
  return running
end

function shutdown(tasklet)
  return true
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end
