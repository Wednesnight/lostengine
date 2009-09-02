local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation

local txtx = windowParams.rect.width-250
local txty = (windowParams.rect.height-13)/2

local c = 254/255

return dcl.rg:Node
{
  name = "rootNode",
  dcl.rg:ClearColor{color = Color(c,c,c,1)},
  dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  dcl.rg:Camera2D
  {
    name = "2D Cam",
    viewport = Rect(0,0,windowParams.rect.width,windowParams.rect.height)
  },
  dcl.rg:DepthTest
  {
    false
  },
  dcl.rg:Draw
  {
    mesh = dcl.mesh:Quad2D
    {
      filename = "down_arrow.png",
      flip = true
    }
  },
  dcl.rg:Draw
  {
    mesh = dcl.font:Text
    {
      font = dcl.font:TrueTypeFont
             {
                filename = "lost/guiro/themes/default/resources/Vera.ttf",
                atlasSize = Vec2(128,64)
             },
      text = "Drop tasklet directory to get started!",
      size =  12,
      transform = MatrixTranslation(Vec3(txtx,txty,0)),
      material = 
      {
          color = Color(0,0,0,1)
      }
    }
  }
}
