startup = function(tasklet) return GuiroTestController:startup(tasklet) end
main = function(tasklet) return GuiroTestController:main(tasklet) end
shutdown = function(tasklet) return GuiroTestController:shutdown(tasklet) end

GuiroTestController = {}

function GuiroTestController:startup(tasklet)
  log.debug("startup")

  self.tasklet = tasklet
  self.dispatcher = tasklet.eventDispatcher

  self.dispatcher:addEventListener(lost.application.ApplicationEvent.QUIT, function(event) self:quit(event) end)
  self.dispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:keyHandler(event) end)
  self.dispatcher:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self:keyHandler(event) end)

--[[ FIXME: we need to resize the window but there's no update method
  if lost.platform.isIPhone() then
    self.windowSize = lost.math.Vec2(320, 480)
  else
    self.windowSize = lost.math.Vec2(800, 600)
  end
  self.window = self.app:createWindow("MainWindow", lost.application.WindowParams("GuiroTest", lost.math.Rect(100, 100, self.windowSize.x, self.windowSize.y)))
]]

  self.controls = {}
--  self.controls.fpsMeter = lost.common.FpsMeter()

  require("lost.io.Guiro")
  self.loader = lost.io.Loader(self.tasklet.loader)
  if lost.platform.isIPhone() then
    self.controls.screen = self.loader:loadScreen("screen-iphone")
  else
    self.controls.screen = self.loader:loadScreen("screen-evtest")
--    self.controls.screen = self.loader:loadScreen("screen")
  end
  self.controls.screen:setEventDispatcher(self.dispatcher)

  -- print out class hierarchy
  log.debug("class hierarchy:")
  lost.guiro.View:printBases()
  lost.guiro.Screen:printBases()
  lost.guiro.UserInterface:printBases()
  lost.guiro.controls.Button:printBases()
  lost.guiro.controls.Window:printBases()

  -- print out view hierarchy
  log.debug("view hierarchy:")
  self.controls.screen:printChildren()

--[[  
  self.renderState = lost.gl.State.create(lost.gl.ClearColor.create(lost.common.Color(1,1,0)),
                                          lost.gl.DepthTest.create(false),
                                          lost.gl.Texture2D.create(false),
                                          lost.gl.VertexArray.create(true))
]]
  self.passedSec = lost.platform.currentTimeSeconds()

  return true
end

function GuiroTestController:main(tasklet)
  local currentSec = lost.platform.currentTimeSeconds()
  local delta = currentSec - self.passedSec

--  self.controls.screen:render(self.window.canvas, true)
--  self.controls.fpsMeter:render(self.windowSize.x - self.controls.fpsMeter.width, 1, self.window.canvas, passedSec)

--[[ FIXME: can only be called within C++ because we loose type information for UiTasklet
  self.tasklet.window.context:swapBuffers()
]]
--	log.debug("interpreter memory usage in kb: "..environment.interpreter:memUsage())

  self.passedSec = currentSec
  return true
end

function GuiroTestController:shutdown(tasklet)
  log.debug("shutdown")
  return true
end

function GuiroTestController:quit(event)
  log.debug("quit")
end

function GuiroTestController:keyHandler(event)
  keyEvent = lost.application.KeyEvent.cast(event)
  
  if (keyEvent.pressed) then
  
    if (keyEvent.key == lost.application.K_ESCAPE) then
      self.dispatcher:dispatchEvent(lost.application.ApplicationEvent(lost.application.ApplicationEvent.QUIT))
    end
  
  end
end
