local MyController = {}

function MyController:init(application)
  -- remember the application
  self.app = application

  -- create our main window
  self.window = application:createWindow("window", lost.application.WindowParams("Application", lost.math.Vec2(800, 600), lost.math.Vec2(100, 100)))

  -- add event listeners
  application:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:keyHandler(event) end)

  -- set our runloop
  application:setRunLoop(function(application) self:run(application) end)
end

function MyController:keyHandler(event)
  local keyEvent = lost.application.KeyEvent.cast(event)
  if (keyEvent.key == 53) then
    self.app:quit()
  end
end

function MyController:run(application)
  log.debug("MyController:run()")
end

function main(application)
  MyController:init(application)
end
