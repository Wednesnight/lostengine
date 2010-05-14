-- Guiroscope main.lua

require("lost.declarative.Context")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.common.Color"

windowParams = WindowParams("Guiroscope", Rect(300,300,800,600))

-- these are deliberately global so we can access them from startup/update/shutdown
dcl = nil
screen = nil

function startup(tasklet)

  log.debug("starting up")

  tasklet.name = "Guiroscope"
  tasklet.waitForEvents = true

  -- running state
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  dcl = lost.declarative.Context(tasklet.loader)
  screen = require("ui")

  return true
end

function update(tasklet)
  return running
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end
