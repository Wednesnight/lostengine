require("settings")
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation

lostrg:Scene
{
  name = "rootNode",
  lostrg:ClearColor{color = Color(0,0,0,1)},
  lostrg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  lostrg:Camera2D
  {
    name = "2D Cam",
    viewport = Rect(0,0,screenSize.width,screenSize.height) -- make this dependant on window size from config file
  },
  lostrg:DepthTest
  {
    false
  },
  lostrg:Draw
  {
    mesh = lostfont:Text
    {
      font = verattf,
      text = "Hello from Lua!",
      size =  12,
      transform = MatrixTranslation(Vec3(200,50,0))
    }
  },      
  lostrg:Draw
  {
    mesh = lostfont:Text
    {
      font = verattf,
      text = "   Subbele, VA filrgb Meine Fresse, es geht! blö",
      size =  20,
      transform = MatrixTranslation(Vec3(10,300,0))
    }
  },      
  lostrg:Draw
  {
    name = "font atlas",
    active = true,
    mesh = lostmesh:Quad2D
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
