local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation

local zimtex = lostgl:Texture
{
  filename = "zim.png"
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
      viewport = Rect(0,0,screenSize.x,screenSize.y)
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
        flip = true,
        size = Vec2(screenSize.x, screenSize.y),
        material = 
        {
          blend = true
        }
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
  }
}
