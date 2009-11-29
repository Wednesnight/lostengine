-- LostEngine main.lua

require("lost.declarative.Context")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local ApplicationEvent = lost.application.ApplicationEvent

windowParams = WindowParams("LostEngine", Rect(50,800,400,128))

-- these are deliberately global so we can access them from startup/update/shutdown
dcl = nil
screen = nil

function startup(tasklet)
  log.debug("starting up")

  -- global reference
  lostengineTasklet = tasklet

  tasklet.waitForEvents = true

  dcl = lost.declarative.Context(tasklet.loader)
  screen = require("ui")
  screen:listenTo(tasklet.eventDispatcher)
  screen.bounds = Bounds(xabs(0), yabs(0), wabs(windowParams.rect.width), habs(windowParams.rect.height))
  -- update needs to be called later since the ui loader code triggers quite a few deferred updates
  callLater(function() screen:updateLayout(true) end) 

  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  return true
end

function update(tasklet)
  processCallLaterQueue()
  screen:updateLayout(false)
  screen.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  return true
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    lostengineTasklet:dispatchApplicationEvent(ApplicationEvent(ApplicationEvent.QUIT))
  end
end
