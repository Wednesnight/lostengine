local _meta = require("_meta")
require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.common.Color"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Vec4"
using "lost.math.Matrix"
using "lost.math.MatrixTranslation"
using "lost.math.MatrixRotX"
using "lost.math.MatrixRotY"
using "lost.math.MatrixScaling"
using "lost.gl.FrameBuffer"

local meshNum = 30
local meshRangeX = Vec2(-3,3)
local meshRangeY = Vec2(-1,3)
local meshRangeZ = Vec2(-3,3)

function createScene(loader)
  local result = {}
  local dcl = lost.declarative.Context(loader)
  
  -- framebuffer setup
  result.fb = FrameBuffer.create(Vec2(_meta.windowRect.width, _meta.windowRect.height),
    gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  result.fbSsao = FrameBuffer.create(Vec2(_meta.windowRect.width, _meta.windowRect.height),
    gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  
  -- light cam used for shadow map calculations
  result.lightCam = dcl.rg:Camera3D
  {
    name = "lightCam",
    viewport = Rect(0, 0, _meta.windowRect.width, _meta.windowRect.height),
    position = Vec3(1,5,1),
    fovY = 45.0,
    depth = Vec2(1.0, 100.0),
    target = Vec3(0,0,0),
    stickToTarget = false
  }
  
  -- shadow projection shader
  local biasMatrix = Matrix()
  biasMatrix:row(0, Vec4(.5,  0,  0, .5))
  biasMatrix:row(1, Vec4( 0, .5,  0, .5))
  biasMatrix:row(2, Vec4( 0,  0, .5, .5))
  biasMatrix:row(3, Vec4( 0,  0,  0,  1))
  result.shadowShader = dcl.gl:Shader
  {
    filename = "shadow",
    params =
    {
      lightPosition = result.lightCam.cam:position(),
      biasMatrix = biasMatrix,
      lightViewMatrix = result.lightCam.cam:viewMatrix(),
      lightProjectionMatrix = result.lightCam.cam:projectionMatrix()
    }
  }

  --ssao shader
  result.ssaoShader = dcl.gl:Shader
  {
    filename = "ssao",
    params =
    {
      lightPosition = result.lightCam.cam:position(),
      biasMatrix = biasMatrix,
      lightViewMatrix = result.lightCam.cam:viewMatrix(),
      lightProjectionMatrix = result.lightCam.cam:projectionMatrix()
    }
  }

  -- scene cam
  result.cam = dcl.rg:Camera3D
  {
    name = "cam",
    viewport = Rect(0, 0, _meta.windowRect.width, _meta.windowRect.height),
    position = Vec3(0,6,6),
    fovY = 45.0,
    depth = Vec2(1.0, 1000.0),
    target = Vec3(0, -2, 0),
    stickToTarget = true
  }
  
  -- meshes
  result.meshes = {}
  table.insert(result.meshes,
  {
    mesh = dcl.mesh:Obj
    {
      filename = "lost/resources/models/magnolia_tri.obj",
      material =
      {
        textures =
        {
          result.fb:depthTexture(),
          result.fbSsao:depthTexture()
        }
      }
    },
    transform = MatrixTranslation(Vec3(math.random(meshRangeX.x, meshRangeX.y),
      math.random(meshRangeY.x, meshRangeY.y), math.random(meshRangeZ.x, meshRangeZ.y)))
  })
  local idx = 1
  while idx < meshNum do
    -- init new mesh instance
    local mesh = lost.mesh.Mesh.create()
    mesh.drawMode = result.meshes[1].mesh.drawMode
    mesh.vertexBuffer = result.meshes[1].mesh.vertexBuffer
    mesh.indexBuffer = result.meshes[1].mesh.indexBuffer
    mesh.material = result.meshes[1].mesh.material
    table.insert(result.meshes,
    {
      mesh = mesh,
      transform = MatrixTranslation(Vec3(math.random(meshRangeX.x, meshRangeX.y),
        math.random(meshRangeY.x, meshRangeY.y), math.random(meshRangeZ.x, meshRangeZ.y)))
    })
    idx = idx + 1
  end

  -- rendergraph node (used for shadow map and resulting scene)
  result.rg = dcl.rg:Node
  {
    name = "rg",
    dcl.rg:ClearColor { color = Color(0,0,0) },
    dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT },
    result.lightCam,
    result.cam,
    dcl.rg:DepthTest { true }
  }

  for k,v in next,result.meshes do
    result.rg:add(dcl.rg:Draw
    {
      mesh = v.mesh
    })
  end

  result.rg:add(dcl.rg:Draw
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
  })

  result.firstPass = 
  {
    -- time delta and angle used for rotation
    passedSec = lost.platform.currentTimeSeconds(),
    angle = 0,
    factor = 1,

    -- prepare stuff for the next render pass
    -- disables cam
    -- enables lightCam
    -- transforms meshes
    -- set shadow map material
    setup = function(self)
      local currentSec = lost.platform.currentTimeSeconds()
      self.angle = math.fmod((currentSec - self.passedSec) * 50 + self.angle, 360)
      local idx = 1
      while idx <= #result.meshes do
        result.meshes[idx].mesh.material.shader = result.shadowShader
        result.meshes[idx].mesh.transform = result.meshes[idx].transform * MatrixRotY(self.factor*self.angle)
        if math.fmod(#result.meshes, 2) == 0 or idx < #result.meshes then
          self.factor = self.factor * -1
        end
        idx = idx + 1
      end
      self.passedSec = currentSec
      
      result.cam.active = false
      result.lightCam.active = true
    end,

    -- process rg node
    process = function(self, context)
      self:setup()
      self.node:process(context)
    end,

    -- scene, surrounded by framebuffer (shadow map)
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
    -- prepare stuff for the next render pass
    -- enables cam
    -- disables lightCam
    setup = function(self)
      result.cam.active = true
      result.lightCam.active = false
    end,

    -- process rg node
    process = function(self, context)
      self:setup()
      self.node:process(context)
    end,

    -- scene, surrounded by framebuffer (ssao)
    node = dcl.rg:Node
    {
      name = "secondPass",
      dcl.rg:FrameBuffer { framebuffer = result.fbSsao },
      result.rg,
      dcl.rg:DefaultFrameBuffer {}
    }
  }

  result.thirdPass = 
  {
    -- prepare stuff for the next render pass
    -- set ssao material
    setup = function(self)
      local idx = 1
      while idx <= #result.meshes do
        result.meshes[idx].mesh.material.shader = result.ssaoShader
        idx = idx + 1
      end
    end,

    -- process rg node
    process = function(self, context, doSsao)
      if doSsao then
        self:setup()
      end
      self.node:process(context)
    end,

    -- scene with applied ssao
    node = dcl.rg:Node
    {
      name = "thirdPass",
      result.rg
    }
  }

  -- optional: draw depth buffer for debugging purposes
  result.debugCam = dcl.rg:Camera2D
  {
    name = "cam",
    viewport = Rect(0, 0, _meta.windowRect.width, _meta.windowRect.height)
  }
  result.debugMesh = dcl.mesh:Quad
  {
    texture = result.fbSsao:depthTexture()
  }
  result.debugNode = dcl.rg:Node
  {
    name = "debug",
    dcl.rg:ClearColor { color = Color(0,0,0) },
    dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT },
    result.debugCam,
    dcl.rg:DepthTest { false },
    dcl.rg:Draw
    {
      name = "quad",
      mesh = result.debugMesh
    }
  }
  return result
end
