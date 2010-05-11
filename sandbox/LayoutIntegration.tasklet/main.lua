using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.application.WindowParams"

screensize = Vec2(320,480)
windowParams = WindowParams("LayoutIntegration", Rect(0,0,screensize.x, screensize.y))

function startup(tasklet)
  log.debug("hello")
  return true
end

function update(tasklet)
  return true
end

function shutdown(tasklet)
  return true
end