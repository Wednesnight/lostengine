local MyController = {}

function MyController:init(application)
  -- remember the application
  self.app = application

  -- create our main window
  self.window = application:createWindow("window", lost.application.WindowParams("Application", lost.math.Vec2(800, 600), lost.math.Vec2(100, 100)))

  -- member setup
  self.passedSec = lost.platform.currentTimeSeconds()
  self.displayAttributes = lost.common.DisplayAttributes()
  self.context = lost.gl.Context(self.displayAttributes)
  self.state = self.context:copyState()
  self.state.clearColor = lost.common.Color(0,0,0,0)
  self.fpsMeter = lost.common.FpsMeter(self.context)

  -- add event listeners
  application:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:keyHandler(event) end)
  application:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self:keyHandler(event) end)
  application:addEventListener(lost.application.MouseEvent.MOUSE_MOVE, function(event) self:mouseHandler(event) end)
  application:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self:mouseHandler(event) end)
  application:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self:mouseHandler(event) end)

  -- set our runloop
  application:setRunLoop(function(application) self:run(application) end)
end

function MyController:keyHandler(event)
  local keyEvent = lost.application.KeyEvent.cast(event)
  log.debug("type: ".. keyEvent.type)
  log.debug("key: ".. keyEvent.key)
  log.debug("character: ".. keyEvent.character)

  if (keyEvent.key == lost.application.K_ESCAPE) then
    self.app:quit()
  end
end

function MyController:mouseHandler(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)
  log.debug("type: ".. mouseEvent.type)
  log.debug("button: ".. tostring(mouseEvent.button))
  log.debug("pos: ".. tostring(mouseEvent.pos))
  log.debug("absPos: ".. tostring(mouseEvent.absPos))
  log.debug("pressed: ".. tostring(mouseEvent.pressed))
end

function MyController:run(application)
  local currentSec = lost.platform.currentTimeSeconds()
  self.window.context:makeCurrent()
  gl.glViewport(0, 0, 800, 600)
  self.context:pushState(self.state)
  self.context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
  self.context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(160,100))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()
  
  self.fpsMeter:render(0, 0, currentSec - self.passedSec);
  self.context:popState()
  self.window.context:swapBuffers()
  self.passedSec = currentSec
end

function main(application)
  MyController:init(application)
end
