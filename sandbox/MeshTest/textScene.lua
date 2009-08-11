local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation

return dcl.rg:Node
{
  name = "rootNode",
  dcl.rg:ClearColor{color = Color(0,0,0,1)},
  dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  dcl.rg:Camera2D
  {
    name = "2D Cam",
    viewport = Rect(0,0,screenSize.width,screenSize.height)
  },
  dcl.rg:DepthTest
  {
    false
  },
  dcl.rg:Draw
  {
    mesh = dcl.font:Text
    {
      font = verattf,
      text = "Hello from Lua!",
      size =  12,
      transform = MatrixTranslation(Vec3(200,50,0))
    }
  },      
  dcl.rg:Draw
  {
    mesh = dcl.font:Text
    {
      font = verattf,
      text = "   Subbele, VA filrgb Meine Fresse, es geht! blö",
      size =  20,
      transform = MatrixTranslation(Vec3(10,300,0))
    }
  },      
  dcl.rg:Draw
  {
    name = "font atlas",
    active = true,
    mesh = dcl.mesh:Quad2D
    {
      texture = verattf.atlas,
      flip = false,
      transform = MatrixTranslation(Vec3(0,0,0)),
      material = 
      {
        blend = true
      }
    }
  }  
}
