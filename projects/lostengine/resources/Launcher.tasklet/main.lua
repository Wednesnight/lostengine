require("lost.declarative.Context")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.application.ApplicationEvent"
using "lost.declarative.Context"


--[[
    setup windowParams
    this should be done in startup, fix this after implementing Window:setParams()
  ]]
windowParams = WindowParams("LostEngine", Rect(50,800,400,128))


--[[
    tasklet startup
  ]]
function startup()

  -- setup tasklet
  tasklet.name = "LostEngine Launcher"
  tasklet.waitForEvents = true

  dcl = Context(tasklet.loader)
  local screen = require("ui")

  -- debug
--  tasklet.renderNode:print()
--  screen:printSubviews()

  -- finally, setup events
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  lost.gl.FrameBuffer.setup()

  return true
end


--[[
    key handler
  ]]
function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.window:close()
  end
end
