function startup(tasklet)
  return Controller:startup(tasklet)
end

function main(tasklet)
  return Controller:main(tasklet)
end

function shutdown(tasklet)
  return Controller:shutdown(tasklet)
end

Controller = {}

function Controller:startup(tasklet)
  log.debug("Controller:startup")
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:keyHandler(event) end)
  return true
end

function Controller:main(tasklet)
--  log.debug("Controller:main")
  return true
end

function Controller:shutdown(tasklet)
  log.debug("Controller:shutdown")
  return true
end

function Controller:keyHandler(event)
  keyEvent = lost.application.KeyEvent.cast(event)
  
  if (keyEvent.pressed) then
  
    if (keyEvent.key == lost.application.K_ESCAPE) then
      -- TODO: quit
    end
  
  end
end
