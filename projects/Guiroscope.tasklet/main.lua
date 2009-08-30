-- Guiroscope main.lua
local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color

require("lost.declarative.Context")

windowParams = WindowParams("Guiroscope", Rect(300,300,640,480))

-- these are deliberately global so we can access them from startup/update/shutdown
dcl = nil
screen = nil

function startup(tasklet)
  log.debug("starting up")
  tasklet.waitForEvents = true
  dcl = lost.declarative.Context(tasklet.loader)

  screen = require("ui")

  screen:listenTo(tasklet.eventDispatcher)
  screen.currentGlobalRect = lost.math.Rect(0,0,windowParams.rect.width, windowParams.rect.height)
  screen:updateLayout() --force update of layout
--  screen:printSubviews()
--  screen.rootNode:print()
  return true
end

function update(tasklet)
  screen.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return true
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end
