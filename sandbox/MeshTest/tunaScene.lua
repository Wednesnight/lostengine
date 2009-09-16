local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local cam3dPosition = Vec3(1,2,2)


local zimtex = dcl.gl:Texture
{
  filename = "zim.png"
}

return dcl.rg:Node
{
  name = "rootNode",
  dcl.rg:ClearColor{ color = Color(0,0,0,1) },
  dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  dcl.rg:Node
  {
    name="2D Background",
    dcl.rg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,640,480) -- make this dependant on window size from config file
    },
    dcl.rg:DepthTest{false},
    dcl.rg:Draw
    {
      name = "little zim",
      active = true,
      mesh = dcl.mesh:Quad2D
      {
        filename = "tunabg.png",
        flip = true
      }
    }
  }
}
