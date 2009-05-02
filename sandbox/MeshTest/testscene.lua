local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color

local cam3dPosition = Vec3(1,2,2)

local lightShader = gl:Shader
{
  filename = "light",
  params = gl:ShaderParams
  {
    LightPosition = camera3D:position(),
    LightDiffuse = Color(1.0, 1.0, 1.0),
    LightAmbient = Color(1.0, 1.0, 1.0),
    LightSpecular = Color(1.0, 1.0, 1.0)
  }
}

rg:Scene
{
  name = "rootNode",
  rg:Node
  {
    name="2D Background",
    rg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,640,480) -- make this dependant on window size from config file
    },
    rg:DepthTest
    {
      false
    },
    rg:Draw
    {
      name = "big zim",
      active = true,
      mesh = mesh:Quad2D
      {
        filename = "zim.png",
        flip = true,
        size = Vec2(640,480)
      }
    },
    rg:Draw
    {
      name = "little red test quad",
      mesh = mesh:Quad2D
      {
        rect = Rect(0,0,3,3),
        material =
        {
          color = Color(1,0,0)
        }
      }
    }    
  },
  rg:Node
  {
    name="3D Foreground",
    rg:Camera3D
    {
      name = "3D Cam",
      viewport = Rect(0,0,640,480), -- make this dependant on window size from config file
      fovY = 45,
      depth = Vec2(1,1000),
      position = cam3dPosition,
      target = Vec3(0,0,0),
      stickToTarget = true
    },
    rg:Draw
    {
      name = "cube",
      mesh = mesh:Obj
      {
        filename = "cube_tri.obj",
        material = 
        {
          color = Color(0,1,0),
          shader = lightShader
        }
      }
    }
  }
}