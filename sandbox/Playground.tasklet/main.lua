function startup()
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  require("lost.guiro")
  lost.guiro.ui():add
  {
    require("playground")
  }

  for f in tasklet.loader:directory("") do
    log.debug(tostring(f))
  end
end

function shutdown()
--  local data = lost.common.Data.create("funky file content")
--  tasklet.writer:write("file.txt", data)
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
