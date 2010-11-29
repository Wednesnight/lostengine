require("lost.declarative.Context")

function startup()
  dcl = lost.declarative.Context(tasklet.loader)
  ui = require("ui")
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  return true
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.window:close()
  end
end
