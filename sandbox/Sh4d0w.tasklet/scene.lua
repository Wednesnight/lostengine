local _meta = require("_meta")
require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.common.Color"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Vec4"
using "lost.math.MatrixTranslation"
using "lost.math.MatrixRotX"
using "lost.math.MatrixScaling"
using "lost.gl.FrameBuffer"

function createScene(loader)
  local result = {}
  local dcl = lost.declarative.Context(loader)
  
  result.fb = FrameBuffer.create(Vec2(_meta.windowRect.width, _meta.windowRect.height),
    gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  
  result.rg = dcl.rg:Node
  {
    name = "scene",
    dcl.rg:ClearColor
    {
      color = Color(0,0,0)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
    },
    dcl.rg:Camera3D
    {
      name = "cam",
      viewport = Rect(0, 0, _meta.windowRect.width, _meta.windowRect.height),
      position = Vec3(1,5,1),
      fovY = 45.0,
      depth = Vec2(1.0, 1000.0),
      target = Vec3(0, 0, 0),
      stickToTarget = false
    },
    dcl.rg:DepthTest
    {
      true
    },
    dcl.rg:Draw
    {
      name = "sphere",
      mesh = dcl.mesh:Sphere
      {
        radius = 1.0,
        material =
        {
          shader = dcl.gl:Shader
          {
            filename = "lost/resources/glsl/basic_light",
            params =
            {
              lightPosition = Vec3(1,5,1)
            }
          }
        }
      }
    }
  }

  result.firstPass = 
  {
    passedSec = lost.platform.currentTimeSeconds(),
    angle = 0,
    setup = function(self)
      local camNode = self.node:recursiveFindByName("cam")
      if camNode ~= nil then
        local currentSec = lost.platform.currentTimeSeconds()
        self.angle = math.fmod((currentSec - self.passedSec) * 50 + self.angle, 360)
        camNode.cam:position(Vec3(1,5,1))
        camNode.cam:rotate(Vec3(0,self.angle,0))
        self.passedSec = currentSec
      end
    end,
    process = function(self, context)
      self.node:process(context)
    end,
    cleanup = function(self, context)
      local camNode = self.node:recursiveFindByName("cam")
      if camNode ~= nil then
        camNode.cam:rotate(Vec3(0,-self.angle,0))
      end
    end,
    node = dcl.rg:Node
    {
      name = "firstPass",
      dcl.rg:FrameBuffer { framebuffer = result.fb },
      result.rg,
      dcl.rg:DefaultFrameBuffer {}
    }
  }

  result.secondPass = 
  {
    setup = function(self)
      local camNode = self.node:recursiveFindByName("cam")
      if camNode ~= nil then
        local plane = self.node:recursiveFindByName("plane")
        if plane ~= nil then
          local textureMatrix = MatrixTranslation(Vec3(.5,.5,.5)) * MatrixScaling(Vec3(.5,.5,.5)) *
            camNode.cam:projectionMatrix()
          plane.mesh.material.shader:enable()
          plane.mesh.material.shader:set("textureMatrix", textureMatrix)
          plane.mesh.material.shader:disable()
        end
        camNode.cam:position(Vec3(3,4,3))
      end
    end,
    process = function(self, context)
      self.node:process(context)
    end,
    node = dcl.rg:Node
    {
      name = "secondPass",
      result.rg,
      dcl.rg:Draw
      {
        name = "plane",
        mesh = dcl.mesh:Quad
        {
          rect = Rect(-5,-5,10,10),
          transform = MatrixTranslation(Vec3(0,-2,0)) * MatrixRotX(90),
          material =
          {
            shader = dcl.gl:Shader
            {
              filename = "shadow"
            },
            textures =
            {
              result.fb:depthTexture()
            }
          }
        }
      }
    }
  }

  result.debugNode = dcl.rg:Node
  {
    name = "debug",
    dcl.rg:ClearColor
    {
      color = Color(0,0,0)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
    },
    dcl.rg:Camera2D
    {
      viewport = Rect(0, 0, _meta.windowRect.width, _meta.windowRect.height)
    },
    dcl.rg:DepthTest
    {
      false
    },
    dcl.rg:Draw
    {
      mesh = dcl.mesh:Quad
      {
        texture = result.fb:depthTexture()
      }
    }
  }
  return result
end
