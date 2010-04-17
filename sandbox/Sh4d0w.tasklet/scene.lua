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
using "lost.math.MatrixRotY"
using "lost.math.MatrixScaling"
using "lost.gl.FrameBuffer"

function createScene(loader)
  local result = {}
  local dcl = lost.declarative.Context(loader)
  
  result.fb = FrameBuffer.create(Vec2(_meta.windowRect.width, _meta.windowRect.height),
    gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  
  result.shadowShader = dcl.gl:Shader
  {
    filename = "shadow",
    params =
    {
      lightPosition = Vec3(1,5,1),
      textureMatrix = lost.math.Matrix()
    }
  }
  
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
      name = "magnolia",
      mesh = dcl.mesh:Obj
      {
        filename = "lost/resources/models/magnolia_tri.obj",
        material =
        {
          shader = result.shadowShader
        }
      }
    },
    dcl.rg:Draw
    {
      name = "magnolia2",
      mesh = dcl.mesh:Obj
      {
        filename = "lost/resources/models/magnolia_tri.obj",
        transform = MatrixTranslation(Vec3(1,2,0)) * MatrixScaling(Vec3(1,1,1)),
        material =
        {
          shader = result.shadowShader
        }
      }
    },
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
        local model = self.node:recursiveFindByName("magnolia")
        if model ~= nil then
          model.mesh.transform = MatrixRotY(self.angle)
        end
        model = self.node:recursiveFindByName("magnolia2")
        if model ~= nil then
          model.mesh.transform = MatrixTranslation(Vec3(1,2,0)) * MatrixScaling(Vec3(1,1,1)) * MatrixRotY(self.angle)
        end
        self.passedSec = currentSec
      end
    end,
    process = function(self, context)
      self.node:process(context)
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
        local textureMatrix = MatrixTranslation(Vec3(.5,.5,.5)) * MatrixScaling(Vec3(.5,.5,.5)) *
          camNode.cam:projectionMatrix()
        result.shadowShader:enable()
        result.shadowShader:set("textureMatrix", textureMatrix)
        result.shadowShader:disable()
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
            shader = result.shadowShader,
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
      name = "cam",
      viewport = Rect(0, 0, _meta.windowRect.width, _meta.windowRect.height)
    },
    dcl.rg:DepthTest
    {
      false
    },
    dcl.rg:Draw
    {
      name = "quad",
      mesh = dcl.mesh:Quad
      {
        texture = result.fb:depthTexture()
      }
    }
  }
  return result
end
