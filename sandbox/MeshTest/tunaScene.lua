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

lostrg:Scene
{
  name = "rootNode",
  lostrg:ClearColor{ color = Color(0,0,0,1) },
  lostrg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  lostrg:Node
  {
    name="2D Background",
    lostrg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,640,480) -- make this dependant on window size from config file
    },
    lostrg:DepthTest{false},
    lostrg:Draw
    {
      name = "little zim",
      active = true,
      mesh = lostmesh:Quad2D
      {
        filename = "tunabg.png",
        flip = true
      }
    }
  }
}
