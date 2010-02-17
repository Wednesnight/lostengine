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

screensize = Vec2(640,480)
windowParams = WindowParams("Sphere", Rect(50,50,screensize.x, screensize.y))
running = true
dcl = nil
rootNode = nil
angle = 0
speed = 20
lasttime = 0

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
      blend = true
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
--    dcl.rg:DepthTest{true},
    dcl.rg:Camera3D
    {
      viewport = Rect(0,0,screensize.x,screensize.y),
      fovy=90,
      depth=Vec2(.01, 1000),
      position = Vec3(0,0,600),
      target=Vec3(0,0,0),
      stickToTarget = true
    },
    dcl.rg:Draw
    {
      name = "circle1",
--      mesh = createDisc()
      mesh = cube.create(200, 11)
    }
  }
  
  circleMesh = rootNode:recursiveFindByName("circle1").mesh
  lasttime = lost.platform.currentTimeMilliSeconds()
  
  lost.application.currentTasklet.renderNode:add(rootNode)
  return true
end

function update(tasklet)
  local now = lost.platform.currentTimeMilliSeconds()
  local delta = now - lasttime
  angle = math.fmod(angle + (delta/speed), 360)
  circleMesh.transform = MatrixRotX(angle)
  lasttime = now
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end
