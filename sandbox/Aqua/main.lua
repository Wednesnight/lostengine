local running = true

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    running = false
  end
end

function startup()
  running = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  
  return running
end

function update()
  return running
end

function shutdown()
end
