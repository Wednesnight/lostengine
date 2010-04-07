require("lost.common.Class")
require("MainView")
require("animation.AnimationHelper")
require("AnimationTest0r")

lost.common.Class "MainController"
{
  running = false,
  fieldSize = lost.math.Vec2(3,3)
}

using "lost.application.K_ESCAPE"
using "lost.application.K_SPACE"
using "lost.application.K_NUMPAD_PLUS"
using "lost.application.K_NUMPAD_MINUS"
using "lost.common.Color"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Vec4"
using "lost.math.Rect"
using "lost.application.ResizeEvent.MAIN_WINDOW_RESIZE"

function MainController:constructor()
  _G.startup = function(tasklet) return self:startup(tasklet) end
  _G.update = function(tasklet) return self:update(tasklet) end
  _G.shutdown = function(tasklet) return self:shutdown(tasklet) end
end

function MainController:startup(tasklet)

  -- init tasklet attributes
  tasklet.name = "TixTixTix"

  -- init members
  self.running = true

  -- init MainView
  self.mainView = MainView(tasklet)

  -- init animation
  self.animation = AnimationTest0r(tasklet)
  self.animation.onFinished = function()
    local startButton = self.mainView.screen("ui")("window")("menu")("buttons")("startButton")
    startButton:title("Start")
    if self.mainView.screen:hidden() then
      self.mainView.screen:hidden(false)
    end
  end

  -- setup ui events
  self.mainView.screen:addEventListener("keyDown", function(event) self:keyHandler(event) end)

  local startButton = self.mainView.screen("ui")("window")("menu")("buttons")("startButton")
  local exitButton = self.mainView.screen("ui")("window")("menu")("buttons")("exitButton")

  startButton:addEventListener("buttonClick", function(event)
    if event.target == event.currentTarget then
      self:toggleMenu()
      self.animation:start()
      event.target:title("Continue")
    end
  end)
  exitButton:addEventListener("buttonClick", function(event)
    if event.target == event.currentTarget then
      self.running = false
    end
  end)

  return self.running
end

function MainController:update(tasklet)
  self.animation:update(tasklet)
  return self.running
end

function MainController:shutdown(tasklet)
  self.animation:stop()
end

function MainController:toggleMenu()
  if self.animation.running then
    self.animation:stop()
  end
  self.mainView.screen:hidden(not self.mainView.screen:hidden())
end

function MainController:keyHandler(event)
  if event.key == K_ESCAPE then
    if self.mainView.screen:hidden() then
      self:toggleMenu()
    end
  end
end
