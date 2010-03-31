require("lost.common.Class")
require("MainView")

lost.common.Class "MainController"
{
  running = false,
  fieldSize = lost.math.Vec2(3,3)
}

using "lost.application.K_ESCAPE"
using "lost.application.K_NUMPAD_PLUS"
using "lost.application.K_NUMPAD_MINUS"
using "lost.common.Color"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.application.ResizeEvent.MAIN_WINDOW_RESIZE"

function MainController:constructor()
  _G.startup = function(tasklet) return self:startup(tasklet) end
  _G.update = function(tasklet) return self:update(tasklet) end
end

function MainController:startup(tasklet)

  -- init tasklet attributes
  tasklet.name = "TixTixTix"

  -- init members
  self.running = true
  self.passedSec = lost.platform.currentTimeSeconds()
  self.angle = 0

  -- init declarative context
  local dcl = Context(tasklet.loader)

  -- init scene
  self.camera = dcl.rg:Camera3D
  {
    name = "camera3d",
    viewport = tasklet.window.params.rect,
    position = Vec3(1, 2, 2),
    fovY = 45.0,
    depth = Vec2(1.0, 1000.0),
    target = Vec3(0, 0, 0),
    stickToTarget = true
  }
  tasklet.eventDispatcher:addEventListener(MAIN_WINDOW_RESIZE, function(event) 
        self.camera.viewport.width = event.width
        self.camera.viewport.height = event.height
      end)

  self.mesh = dcl.mesh:Obj
  {
    filename = "magnolia_tri.obj",
    material =
    {
      shader = dcl.gl:Shader
      {
        filename = "light"
      }
    }
  }
  
  self.scene = dcl.rg:Node
  {
    dcl.rg:ClearColor
    {
      color = Color(0,0,0)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
    },
    self.camera,
    dcl.rg:DepthTest
    {
      true
    },
    dcl.rg:Draw
    {
      name = "mesh",
      mesh = self.mesh
    }
  }
--  tasklet.renderNode:add(self.scene)

  -- init MainView
  self.mainView = MainView(tasklet)

  -- setup ui events
  self.mainView.screen:addEventListener("keyDown", function(event) self:keyHandler(event) end)
  local exitButton = self.mainView.screen("ui")("window")("menu")("buttons")("exitButton")
  exitButton:addEventListener("buttonClick", function(event) self.running = false end)

  return self.running
end

function MainController:update(tasklet)

  if self.running then
    local currentSec = lost.platform.currentTimeSeconds()
    local delta = currentSec - self.passedSec

    self.angle = math.fmod(delta * 50 + self.angle, 360)
    self.mesh.transform = lost.math.MatrixRotX(self.angle) * lost.math.MatrixRotY(self.angle)

    self.passedSec = currentSec
  end

  -- render scene
  self.scene:process(tasklet.window.context)
  
  return self.running
end

function MainController:keyHandler(event)

  if event.key == K_ESCAPE then
    self.mainView.screen:hidden(not self.mainView.screen:hidden())
  end

end
