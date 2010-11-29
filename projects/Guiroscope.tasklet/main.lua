-- Guiroscope main.lua

require("lost.declarative.Context")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.common.Color"

-- these are deliberately global so we can access them from startup/update/shutdown
dcl = nil
ui = nil

function startup()
  log.debug("starting up")

  -- running state
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  dcl = lost.declarative.Context(tasklet.loader)
  ui = require("ui")
  ui:printSubviews()

  return true
end

function update()
  return running
end

function shutdown()
  log.debug("shutting down")
  return true
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end
