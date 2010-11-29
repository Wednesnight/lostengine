require("lost.common.Class")
require("lost.declarative.Context")
require("animation.AnimationHelper")

lost.common.Class "AnimationTest0r" {}

using "lost.declarative.Context"
using "lost.common.Color"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Vec4"
using "lost.math.Rect"

function AnimationTest0r:constructor(tasklet)

  self.running = false

  -- init declarative context
  local dcl = Context(tasklet.loader)

  -- init scene
  self.cam2d = dcl.rg:Camera2D
  {
    viewport = Rect(0,0,tasklet.window.params.rect.width,tasklet.window.params.rect.height),
  }
  self.cam3d = dcl.rg:Camera3D
  {
    name = "camera3d",
    viewport = Rect(0,0,tasklet.window.params.rect.width,tasklet.window.params.rect.height),
    position = Vec3(1, 2, 2),
    fovY = 45.0,
    depth = Vec2(0.1, 1000.0),
    target = Vec3(0, 0, 0),
    stickToTarget = true
  }
  tasklet.eventDispatcher:addEventListener(MAIN_WINDOW_RESIZE, function(event)
    local viewport = Rect(0,0,event.width,event.height)
    self.cam2d.cam:viewport(viewport)
    self.cam3d.cam:viewport(viewport)
    self.blank.mesh:updateSize(viewport)
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

  self.blank = dcl.mesh:Quad
  {
    rect = Rect(0,0,tasklet.window.params.rect.width,tasklet.window.params.rect.height),
    material =
    {
      blend = true,
      color = Color(1,1,1,1)
    }
  }

  self.blankNode = dcl.rg:Node
  {
    active = false,
    self.cam2d,
    dcl.rg:Draw
    {
      name = "blank",
      mesh = self.blank
    },
    self.cam3d
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
    self.cam3d,
    dcl.rg:DepthTest
    {
      true
    },
    dcl.rg:Draw
    {
      name = "mesh",
      mesh = self.mesh
    },
    self.blankNode
  }
  
  self:setupAnimation()
end

function AnimationTest0r:setupAnimation()

  self.animations = {}
  self.pos = 0

  local a = lost.animation.AnimationHelper.interpolateVec3(Vec3(), Vec3(0,-360,0), 10)
  a:addEventListener("animationStart", function(animation)
    self:next()
  end)
  a:addEventListener("animationUpdate", function(animation)
    local matrix = lost.math.MatrixRotX(animation.currentValue.x) *
                   lost.math.MatrixRotY(animation.currentValue.y) *
                   lost.math.MatrixRotZ(animation.currentValue.z)
    self.mesh.transform = matrix
  end)
  a:addEventListener("animationFinished", function(animation)
    animation:start()
  end)
  table.insert(self.animations, a)
  a = lost.animation.AnimationHelper.interpolateVec4(Vec4(1,1,1,1), Vec4(0,0,0,0), 1)
  a:addEventListener("animationStart", function(animation)
    self.blankNode.active = true
    self:next()
  end)
  a:addEventListener("animationUpdate", function(animation)
    local color = lost.common.Color(animation.currentValue.x,animation.currentValue.y,animation.currentValue.z,animation.currentValue.w)
    self.blank.material.color = color
  end)
  a:addEventListener("animationFinished", function(animation)
    self.blankNode.active = false
  end)
  table.insert(self.animations, a)
  a = lost.animation.AnimationHelper.interpolateVec3(self.cam3d.cam:position(), Vec3(1,1,1), 3)
  a:addEventListener("animationUpdate", function(animation)
    self.cam3d.cam:position(animation.currentValue)
  end)
  a:addEventListener("animationFinished", function(animation)
    self:next()
  end)
  table.insert(self.animations, a)
  a = lost.animation.AnimationHelper.interpolateVec4(Vec4(1,1,1,1), Vec4(0,0,0,0), 1)
  a:addEventListener("animationStart", function(animation)
    self.blankNode.active = true
    self.cam3d.cam:position(Vec3(0,.25,.25))
    self:next()
  end)
  a:addEventListener("animationUpdate", function(animation)
    local color = lost.common.Color(animation.currentValue.x,animation.currentValue.y,animation.currentValue.z,animation.currentValue.w)
    self.blank.material.color = color
  end)
  a:addEventListener("animationFinished", function(animation)
    self.blankNode.active = false
  end)
  table.insert(self.animations, a)
  a = lost.animation.AnimationHelper.interpolateVec4(Vec4(1,1,1,1), Vec4(1,0,0,1), 3)
  a:addEventListener("animationUpdate", function(animation)
    local color = Color(animation.currentValue.x,animation.currentValue.y,animation.currentValue.z,animation.currentValue.w)
    self.mesh.material.color = color
  end)
  a:addEventListener("animationFinished", function(animation)
    self:next()
  end)
  table.insert(self.animations, a)
  a = lost.animation.AnimationHelper.interpolateVec4(Vec4(1,0,0,1), Vec4(1,1,1,1), 3)
  a:addEventListener("animationUpdate", function(animation)
    local color = Color(animation.currentValue.x,animation.currentValue.y,animation.currentValue.z,animation.currentValue.w)
    self.mesh.material.color = color
  end)
  a:addEventListener("animationFinished", function(animation)
    self:next()
  end)
  table.insert(self.animations, a)
end

function AnimationTest0r:update(tasklet)
  self.scene:process(tasklet.window.context)
end

function AnimationTest0r:start()
  self.running = true
  if self.current ~= nil then
    self.current:start()
  else
    self:next()
  end
end

function AnimationTest0r:stop()
  if self.running then
    if self.current ~= nil then
      self.current:stop()
    end
    self.running = false
  end
end

function AnimationTest0r:reset()
  self.pos = 0
  self.current = nil
  self.cam3d.cam:position(Vec3(1, 2, 2))
  for k,a in next,self.animations do
    a:stop()
    lost.animation.AnimationHelper.resetInterpolation(a)
  end
end

function AnimationTest0r:next()
  if self.running then
    if self.pos < table.maxn(self.animations) then
      self.pos = self.pos + 1
      self.current = self.animations[self.pos]
      self.current:start()
    else
      self:stop()
      self:reset()
      if self.onFinished ~= nil then
        self.onFinished()
      end
    end
  end
end
