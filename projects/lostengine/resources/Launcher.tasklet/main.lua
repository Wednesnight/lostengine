require("lost.declarative.Context")

using "lost.application.WindowParams"
using "lost.math.Rect"
using "lost.application.ApplicationEvent"
using "lost.declarative.Context"

--[[
    tasklet startup
  ]]
function startup()
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
