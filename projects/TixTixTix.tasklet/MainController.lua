require("lost.common.Class")
require("lost.common.CallLater")
require("MainView")
require("animation.AnimationHelper")

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
    local viewport = lost.math.Rect(0,0,event.width,event.height)
    self.camera.cam:viewport(viewport)
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
  local newGameButton = self.mainView.screen("ui")("window")("menu")("buttons")("newGameButton")
  newGameButton:addEventListener("buttonClick", function(event) if event.target == event.currentTarget then self:toggleMenu() end end)
  local exitButton = self.mainView.screen("ui")("window")("menu")("buttons")("exitButton")
  exitButton:addEventListener("buttonClick", function(event) if event.target == event.currentTarget then self.running = false end end)

  -- setup animation
  local animationDuration = 5
  -- rotation
  self.animation = lost.animation.AnimationHelper.interpolateVec3(Vec3(), Vec3(-360,0,0), animationDuration)
  self.animation:addEventListener("animationUpdate", function(animation)
    local matrix = lost.math.MatrixRotX(animation.currentValue.x) *
                   lost.math.MatrixRotY(animation.currentValue.y) *
                   lost.math.MatrixRotZ(animation.currentValue.z)
    self.mesh.transform = matrix
  end)
  self.animation:addEventListener("animationFinished", function(animation)
    animation:start()
  end)
  -- color
  local startColor = Vec4(self.mesh.material.color.r, self.mesh.material.color.g, self.mesh.material.color.b, self.mesh.material.color.a)
  local targetColor = Vec4(1,0,0,1)
  self.animation2 = lost.animation.AnimationHelper.interpolateVec4(startColor,targetColor, animationDuration/2)
  self.animation2:addEventListener("animationUpdate", function(animation)
    local color = lost.common.Color(animation.currentValue.x,animation.currentValue.y,animation.currentValue.z,animation.currentValue.w)
    self.mesh.material.color = color
  end)
  self.animation2:addEventListener("animationFinished", function(animation)
    lost.animation.AnimationHelper.reverseInterpolation(animation)
    animation:start()
  end)
  -- move
  local startPos = Vec3(0,0,0)
  local targetPos = Vec3(-2,0,0)
  local undo = true
  self.animation3 = lost.animation.AnimationHelper.interpolateVec3(startPos, targetPos, animationDuration/2)
  self.animation3:addEventListener("animationUpdate", function(animation)
    self.mesh.transform = self.mesh.transform * lost.math.MatrixTranslation(animation.currentValue)
  end)
  self.animation3:addEventListener("animationFinished", function(animation)
    lost.animation.AnimationHelper.reverseInterpolation(animation)
    animation:start()
  end)

  return self.running
end

function MainController:update(tasklet)

  -- render scene
  self.scene:process(tasklet.window.context)
  
  return self.running
end

function MainController:toggleMenu()
  self.mainView.screen:hidden(not self.mainView.screen:hidden())
  if self.animation.running then
    self.animation:stop()
  else
    self.animation:start()
  end
  if self.animation2.running then
    self.animation2:stop()
  else
    self.animation2:start()
  end
  if self.animation3.running then
    self.animation3:stop()
  else
    self.animation3:start()
  end
end

function MainController:keyHandler(event)
  if event.key == K_ESCAPE then
    self:toggleMenu()
  end
end
