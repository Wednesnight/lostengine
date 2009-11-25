local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local cam3dPosition = Vec3(0,0,10)

local lightShader = dcl.gl:Shader
{
  filename = "light",
  params = dcl.gl:ShaderParams
  {
    LightPosition = cam3dPosition,
    LightDiffuse = Color(1.0, 1.0, 1.0),
    LightAmbient = Color(1.0, 1.0, 1.0),
    LightSpecular = Color(1.0, 1.0, 1.0)
  }
}

return dcl.rg:Node
{
  name = "3D Scene",
  dcl.rg:ClearColor{color = Color(0,0,0,1)},
  dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  dcl.rg:Node
  {
    name="3D Foreground",
    dcl.rg:Camera3D
    {
      name = "3D Cam",
      viewport = Rect(0,0,screenSize.x,screenSize.y),
      fovY = 45,
      depth = Vec2(.1,100),
      position = cam3dPosition,
      target = Vec3(0,0,0),
      stickToTarget = true
    },
    dcl.rg:DepthTest
    {
      true
    },    
    dcl.rg:Draw
    {
      name = "cube",
      mesh = dcl.mesh:Obj
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