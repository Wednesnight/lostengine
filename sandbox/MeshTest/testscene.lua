local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation

local zimtex = dcl.gl:Texture
{
  filename = "zim.png"
}

return dcl.rg:Node
{
  name = "rootNode",
  dcl.rg:ClearColor{color = Color(0,0,0,1)},
  dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  dcl.rg:Node
  {
    name="2D Background",
    dcl.rg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,screenSize.x,screenSize.y)
    },
    dcl.rg:DepthTest
    {
      false
    },
    dcl.rg:Draw
    {
      name = "big zim",
      active = true,
      mesh = dcl.mesh:Quad2D
      {
        texture = zimtex,
        flip = true,
        size = Vec2(screenSize.x, screenSize.y),
        material = 
        {
          blend = true
        }
      }
    },
    dcl.rg:Draw
    {
      name = "little zim",
      active = true,
      mesh = dcl.mesh:Quad2D
      {
        texture = zimtex,
        flip = true,
        transform = MatrixTranslation(Vec3(400, 370, 0))
      }
    },
    dcl.rg:Draw
    {
      name = "little red test quad",
      mesh = dcl.mesh:Quad2D
      {
        rect = Rect(0,0,3,3),
        material =
        {
          color = Color(1,0,0)
        }
      }
    }    
  }
}
