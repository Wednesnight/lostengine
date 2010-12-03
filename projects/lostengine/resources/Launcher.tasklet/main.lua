
require("lost.guiro.view.UserInterface")

function drop(event)
  tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(event.filename))
end

function startup()
  local ui = lost.guiro.ui()
  ui:addEventListener("drop", drop)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  return true
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.window:close()
  end
end
