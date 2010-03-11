-- procedural textures test program
-- quad.png is only loaded because we need a Quad2D with texture coordinates 
-- and creating one with a bitmap is presently the only way we can do that from Lua

require "lost.declarative.Context"

local loadShader = lost.gl.loadShader
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local Color = lost.common.Color
local Camera2D = lost.camera.Camera2D
local Camera3D = lost.camera.Camera3D
local WindowParams = lost.application.WindowParams
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotX = lost.math.MatrixRotX
local MatrixRotZ = lost.math.MatrixRotZ

screensize = Vec2(320,480)
windowParams = WindowParams("Sphere", Rect(50,50,screensize.x, screensize.y))
running = true
dcl = nil
rootNode = nil
angle = 0
speed = 50
lasttime = 0

globalRadius = 200
numSlices = 15

accumulateFrames = 100
currentFrameNum = 0
frameTime = 0

-- creates a quad around 0,0,0 in xz plane with given side length
require("cube")

function createDisc()
  return dcl.mesh:Quad
  {
    filename = "quad.png", 
    material = 
    {
      shader = dcl.gl:Shader
      {
        filename = "disc"
      },
      color = Color(1,.5,0),
      blend = true,
    }
  }
end

function startup(tasklet)
  tasklet.name = "Sphere"
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  dcl = lost.declarative.Context(tasklet.loader)
    
  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor
    {
      color = Color(1,1,1,1)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
    },
    dcl.rg:DepthTest{true},
    dcl.rg:Camera3D
    {
      name = "cam",
      viewport = Rect(0,0,screensize.x,screensize.y),
      fovy=90,
      depth=Vec2(.01, 4000),
      position = Vec3(0,0,370),
      target=Vec3(0,0,0),
      stickToTarget = true
    },
    dcl.rg:Draw
    {
      name = "circle1",
      mesh = cube.create(globalRadius, numSlices),
      active = true
    },
    dcl.rg:Draw
    {
      name = "circle2",
      mesh = cube.create(globalRadius, numSlices),
      active = true
    }    
  }
  
  circleMesh1 = rootNode:recursiveFindByName("circle1").mesh
  circleMesh2 = rootNode:recursiveFindByName("circle2").mesh
  lasttime = lost.platform.currentTimeMilliSeconds()
  
  lost.application.currentTasklet.renderNode:add(rootNode)
  cam = rootNode:recursiveFindByName("cam").cam

  if(lost.platform.isIPhone()) then
    log.debug("yes, we'Re on iPhone")
  end

  return true
end

function update(tasklet)
  local now = lost.platform.currentTimeMilliSeconds()
  local delta = now - lasttime
  angle = math.fmod(angle + (delta/speed), 360)
  local mm = MatrixRotX(angle)*MatrixRotZ(angle)
  circleMesh1.transform = mm*MatrixRotX(0)
  circleMesh2.transform = mm*MatrixRotZ(-90)
  lasttime = now

  frameTime = frameTime + delta
  currentFrameNum = currentFrameNum + 1
  if currentFrameNum == accumulateFrames then 
    local fps = 1000/(frameTime/accumulateFrames)
    log.debug("average fps over "..accumulateFrames.." frames: "..fps)
    frameTime = 0
    currentFrameNum = 0
  end

  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  elseif (event.key == lost.application.K_A) then
    local v = cam:position()
    v.z = v.z-10
    log.debug(v.z)
    cam:position(v)
  end
end
