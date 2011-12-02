require("lost.common.Shaders")
require("lost.shaders.ShaderFactory")

local Color = lost.common.Color
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Vec4 = lost.math.Vec4
local Matrix = lost.math.Matrix
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotX = lost.math.MatrixRotX
local MatrixRotY = lost.math.MatrixRotY
local MatrixScaling = lost.math.MatrixScaling
local FrameBuffer = lost.gl.FrameBuffer
local ShaderFactory = lost.shaders.ShaderFactory

local meshNum = 25
local meshRangeX = Vec2(-3,3)
local meshRangeY = Vec2(-1,3)
local meshRangeZ = Vec2(-3,3)

function createScene(loader)
  local result = {}
  
  -- framebuffer setup
  result.fb = FrameBuffer.create(Vec2(config.window.width, config.window.height),
    gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  result.fbSsao = FrameBuffer.create(Vec2(config.window.width, config.window.height),
    gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)

  -- light cam used for shadow map calculations
  result.lightCam = lost.rg.Camera.create(lost.camera.Camera3D.create(Rect(0, 0, config.window.width, config.window.height)))
  result.lightCam.name = "lightCam"
  result.lightCam.cam:position(Vec3(1,5,1))
  result.lightCam.cam:depth(Vec2(1.0, 1000.0))
  result.lightCam.cam:target(Vec3(0,0,0))
  result.lightCam.cam:fovY(45.0)
  result.lightCam.cam:stickToTarget(false)
  
  -- shader
  local shaderFactory = ShaderFactory(loader)
  result.ssaoShader = shaderFactory:newInstance("shader")

  result.shaderParams = lost.gl.UniformBlock.create()
  result.shaderParams:setBool("lightingEnabled", true)
  result.shaderParams:setBool("ssaoEnabled", true)
  result.shaderParams:setBool("shadowmapEnabled", true)
  result.shaderParams:setBool("matcapEnabled", true)
  result.shaderParams:set("lightPosition", result.lightCam.cam:position())
  result.shaderParams:set("lightViewMatrix", result.lightCam.cam:viewMatrix())
  result.shaderParams:set("lightProjectionMatrix", result.lightCam.cam:projectionMatrix())

  -- scene cam
  result.cam = lost.rg.Camera.create(lost.camera.Camera3D.create(Rect(0, 0, config.window.width, config.window.height)))
  result.cam.name = "cam"
  result.cam.cam:position(Vec3(0,5,5))
  result.cam.cam:depth(Vec2(1.0, 1000.0))
  result.cam.cam:target(Vec3(0,-2,0))
  result.cam.cam:fovY(45.0)
  result.cam.cam:stickToTarget(true)

  -- meshes
  result.meshes = {}
  local mesh = lost.mesh.Loader.obj(loader:load("lost/resources/models/magnolia_tri.obj"))
  mesh.material.shader = result.ssaoShader
  mesh.material.uniforms = result.shaderParams
  mesh.material:addTexture(result.fb:depthTexture())
  mesh.material:addTexture(result.fbSsao:depthTexture())
  local bmp = lost.bitmap.Bitmap.create(loader:load("materials/gold.tga"))
  bmp:premultiplyAlpha()
  local params = lost.gl.Texture.Params()
  params.minFilter = gl.GL_LINEAR
  params.magFilter = gl.GL_LINEAR
  mesh.material:addTexture(lost.gl.Texture.create(bmp, params))
  table.insert(result.meshes, {
    mesh = mesh,
    transform = MatrixTranslation(Vec3(math.random(meshRangeX.x, meshRangeX.y),
      math.random(meshRangeY.x, meshRangeY.y), math.random(meshRangeZ.x, meshRangeZ.y)))
  })

  local idx = 1
  while idx < meshNum do
    -- init new mesh instance
    local mesh = lost.mesh.Mesh.create()
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

  result.setMatCap = function(self, data)
    for k,m in next,self.meshes do
      m.mesh.material = lost.mesh.Material.create()
      m.mesh.material.shader = self.ssaoShader
      m.mesh.material.uniforms = self.shaderParams
      m.mesh.material:addTexture(self.fb:depthTexture())
      m.mesh.material:addTexture(self.fbSsao:depthTexture())
      local params = lost.gl.Texture.Params()
      params.minFilter = gl.GL_LINEAR
      params.magFilter = gl.GL_LINEAR
      m.mesh.material:addTexture(lost.gl.Texture.create(data, params))
    end
  end

  -- rendergraph node (used for shadow map and resulting scene)
  result.rg = lost.rg.Node.create()
  result.rg.name = "rg",
  result.rg:add(lost.rg.ClearColor.create(Color(0,0,0)))
  result.rg:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  result.rg:add(result.lightCam)
  result.rg:add(result.cam)
  result.rg:add(lost.rg.DepthTest.create(true))

  for k,v in next,result.meshes do
    result.rg:add(lost.rg.Draw.create(v.mesh))
  end

  local plane = lost.mesh.Quad.create(Rect(-5,-5,10,10))
  plane.name = "plane"
  plane.transform = MatrixTranslation(Vec3(0,-2,0)) * MatrixRotX(90)
  plane.material.shader = result.ssaoShader
  plane.material.uniforms = result.shaderParams
  plane.material:addTexture(result.fb:depthTexture())
  plane.material:addTexture(result.fbSsao:depthTexture())
  bmp = lost.bitmap.Bitmap.create(loader:load("materials/white.png"))
  bmp:premultiplyAlpha()
  params = lost.gl.Texture.Params()
  params.minFilter = gl.GL_LINEAR
  params.magFilter = gl.GL_LINEAR
  plane.material:addTexture(lost.gl.Texture.create(bmp, params))
  result.rg:add(lost.rg.Draw.create(plane))

  result.lightingEnabled = true
  result.shadowmapEnabled = true
  result.ssaoEnabled = true
  result.matcapEnabled = true

  local firstPassNode = lost.rg.Node.create()
  firstPassNode.name = "firstPass"
  firstPassNode:add(lost.rg.FrameBuffer.create(result.fb))
  firstPassNode:add(result.rg)
  firstPassNode:add(lost.rg.DefaultFrameBuffer.create())
  tasklet.renderNode:add(firstPassNode)
  
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
    -- disable ssao
    setup = function(self)
      local currentSec = lost.platform.currentTimeSeconds()
      self.angle = math.fmod((currentSec - self.passedSec) * 50 + self.angle, 360)
      local idx = 1
      while idx <= #result.meshes do
        result.meshes[idx].mesh.transform = result.meshes[idx].transform * MatrixRotY(self.factor*self.angle)
        if math.fmod(#result.meshes, 2) == 0 or idx < #result.meshes then
          self.factor = self.factor * -1
        end
        idx = idx + 1
      end
      self.passedSec = currentSec
      
      result.shaderParams:setBool("lightingEnabled", false)
      result.shaderParams:setBool("shadowmapEnabled", false)
      result.shaderParams:setBool("ssaoEnabled", false)
      result.shaderParams:setBool("matcapEnabled", false)
      result.cam.active = false
      result.lightCam.active = true
    end,

    -- process rg node
    process = function(self, context)
      self:setup()
    end,

    -- scene, surrounded by framebuffer (shadow map)
    node = firstPassNode
  }

  local secondPassNode = lost.rg.Node.create()
  secondPassNode.name = "secondPass"
  secondPassNode:add(lost.rg.FrameBuffer.create(result.fbSsao))
  secondPassNode:add(result.rg)
  secondPassNode:add(lost.rg.DefaultFrameBuffer.create())
  tasklet.renderNode:add(secondPassNode)

  result.secondPass = 
  {
    -- prepare stuff for the next render pass
    -- enables cam
    -- disables lightCam
    setup = function(self)
      result.shaderParams:setBool("lightingEnabled", result.lightingEnabled)
      result.shaderParams:setBool("shadowmapEnabled", result.shadowmapEnabled)
      result.shaderParams:setBool("matcapEnabled", result.matcapEnabled)
      result.cam.active = true
      result.lightCam.active = false
    end,

    -- process rg node
    process = function(self, context)
      self:setup()
    end,

    -- scene, surrounded by framebuffer (ssao)
    node = secondPassNode
  }

  local thirdPassNode = lost.rg.Node.create()
  thirdPassNode.name = "thirdPass"
  thirdPassNode:add(result.rg)
  tasklet.renderNode:add(thirdPassNode)

  result.thirdPass = 
  {
    -- prepare stuff for the next render pass
    -- enable ssao
    setup = function(self)
      result.shaderParams:setBool("ssaoEnabled", result.ssaoEnabled)
    end,

    -- process rg node
    process = function(self, context)
      self:setup()
    end,

    -- scene with applied ssao
    node = thirdPassNode
  }

  -- optional: draw depth buffer for debugging purposes
  result.debugCam = lost.rg.Camera.create(lost.camera.Camera2D.create(Rect(0, 0, config.window.width, config.window.height)))
  result.debugCam.name = "cam"
  result.debugMesh = lost.mesh.Quad.create(result.fb:depthTexture(), false)
  result.debugMesh.material.shader = lost.common.Shaders.textureShader()
  result.debugNode = lost.rg.Node.create()
  result.debugNode.name = "debug"
  result.debugNode.active = false
  result.debugNode:add(lost.rg.ClearColor.create(Color(0,0,0)))
  result.debugNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT))
  result.debugNode:add(result.debugCam)
  result.debugNode:add(lost.rg.DepthTest.create(false))
  result.debugNode:add(lost.rg.Draw.create(result.debugMesh))
  tasklet.renderNode:add(result.debugNode)

  return result
end
