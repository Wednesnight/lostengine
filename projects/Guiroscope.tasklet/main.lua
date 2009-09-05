-- Guiroscope main.lua
local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color

require("lost.declarative.Context")

windowParams = WindowParams("Guiroscope", Rect(300,300,800,600))

-- these are deliberately global so we can access them from startup/update/shutdown
dcl = nil
screen = nil
numUpdateLayout = 0
function incUp()
  numUpdateLayout = numUpdateLayout + 1
end

function startup(tasklet)
  log.debug("starting up")
  tasklet.waitForEvents = true
  dcl = lost.declarative.Context(tasklet.loader)

  screen = require("ui")

  -- FIXME: the following few lines could be put into a helpfer function in Screen
  -- that associates a Screen instance with a tasklet and performs some final updates and an initial redraw
  -- then again a redraw might not be what you want in all use cases
  screen:listenTo(tasklet.eventDispatcher)
  screen.currentGlobalRect = lost.math.Rect(0,0,windowParams.rect.width, windowParams.rect.height)
  -- update needs to be called later since the ui loader code triggers quite a few deferred updates
  callLater(function() screen:updateLayout() end) 

  -- running state
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

-- DEBUG    
  callLater(function()
      numUpdateLayout = 0
      screen:printSubviews()
      log.debug("number of views: "..numUpdateLayout)
    end)
--  callLater(function() screen.rootNode:print()  end)
  return true
end

function update(tasklet)
  processCallLaterQueue()
  screen.rootNode:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  if numUpdateLayout > 0 then
    log.debug("--------- updates: "..numUpdateLayout)
    numUpdateLayout = 0
  end
  return running
end

function shutdown(tasklet)
  log.debug("shutting down")
  return true
end

function keyHandler(event)
  local keyEvent = lost.application.KeyEvent.cast(event)
  if keyEvent.key == lost.application.K_ESCAPE then
    running = false
  end
end
