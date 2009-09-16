-- Blurr3r
require("lost.common.CallLater")
require("lost.declarative.Context")

local WindowParams = lost.application.WindowParams
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixRotY = lost.math.MatrixRotY
local MatrixRotZ = lost.math.MatrixRotZ
local Color = lost.common.Color


windowParams = WindowParams("Blurr3r", Rect(300,300, 800, 600))

dcl = nil
scene = nil
mesh = nil
rotY = 0
rotZ = 0
step = .2

function startup(tasklet)
  dcl = lost.declarative.Context(tasklet.loader)
  scene = dcl.rg:Node
  {
    dcl.rg:Camera3D
    {
      viewport = Rect(0,0,800,600),
      fovy = 45,
      depth = Vec2(.1, 1000),
      position = Vec3(0,0,-10),
      target = Vec3(0,0,0),
      stickToTarget = false
    },
    dcl.rg:ClearColor{color=Color(0,0,0,1)},
    dcl.rg:Clear{mask=gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT},
    dcl.rg:DepthTest{true},
    dcl.rg:Draw
    {
      name = "cube",
      mesh = dcl.mesh:Obj
      {
        filename = "cube_tri.obj",
        transform = MatrixRotZ(45),
        material = 
        {
          color = Color(0,1,0),
--          shader = lightShader
        }
      }
    }    
  }
  
  mesh = scene:recursiveFindByName("cube")
  
  return true
end

function update(tasklet)
  rotY = math.fmod(rotY+step, 360)
  rotZ = math.fmod(rotZ+step, 360)
  mesh.transform = MatrixRotY(rotY)*MatrixRotZ(rotZ)
  processCallLaterQueue()
  scene:process(tasklet.window.context)
  tasklet.window.context:swapBuffers()
  log.debug("up")
  return true
end

function shutdown(tasklet)
  return true
end