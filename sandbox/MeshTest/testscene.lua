require("settings")

local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local cam3dPosition = Vec3(1,2,2)


local zimtex = lostgl:Texture
{
  filename = "zim.png"
}

local lightShader = lostgl:Shader
{
  filename = "light",
  params = lostgl:ShaderParams
  {
    LightPosition = cam3dPosition,
    LightDiffuse = Color(1.0, 1.0, 1.0),
    LightAmbient = Color(1.0, 1.0, 1.0),
    LightSpecular = Color(1.0, 1.0, 1.0)
  }
}

lostrg:Scene
{
  name = "rootNode",
  lostrg:ClearColor{color = Color(0,0,0,1)},
  lostrg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  lostrg:Node
  {
    name="2D Background",
    lostrg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,640,480) -- make this dependant on window size from config file
    },
    lostrg:DepthTest
    {
      false
    },
    lostrg:Draw
    {
      name = "big zim",
      active = true,
      mesh = lostmesh:Quad2D
      {
        texture = zimtex,
        flip = false,
        size = Vec2(640,480)
      }
    },
    lostrg:Draw
    {
      name = "little zim",
      active = true,
      mesh = lostmesh:Quad2D
      {
        texture = zimtex,
        flip = true,
        transform = MatrixTranslation(Vec3(400, 370, 0))
      }
    },
    lostrg:Draw
    {
      name = "little red test quad",
      mesh = lostmesh:Quad2D
      {
        rect = Rect(0,0,3,3),
        material =
        {
          color = Color(1,0,0)
        }
      }
    }    
  },
  lostrg:Node
  {
    name="3D Foreground",
    lostrg:Camera3D
    {
      name = "3D Cam",
      viewport = Rect(0,0,640,480), -- make this dependant on window size from config file
      fovY = 45,
      depth = Vec2(1,1000),
      position = cam3dPosition,
      target = Vec3(0,0,0),
      stickToTarget = true
    },
    lostrg:DepthTest
    {
      true
    },    
    lostrg:Draw
    {
      name = "cube",
      mesh = lostmesh:Obj
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
