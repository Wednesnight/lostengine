
local Rect = lost.math.Rect
local WindowParams = lost.application.WindowParams
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local MatrixTranslation = lost.math.MatrixTranslation
local Color = lost.common.Color

local verattf = dcl.font:TrueTypeFont
       {
         filename = "Vera.ttf",
         atlasSize = Vec2(256,256)
       }

local fontColor = Color(0,0,0,1)
local off = 1
local shadowOffset = Vec3(off, -off, 0)
local shadowColor = Color(0,0,0,.3)
local fontSize = 40

return dcl.rg:Node
{
  name = "rootNode",
  dcl.rg:ClearColor{color = Color(1,1,1,1)},
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
  -- draw shadow first
  dcl.rg:Draw
  {
    mesh = dcl.font:Text
    {
      font = verattf,
      text = "Drop tasklet directory to get started!",
      size =  fontSize,
      transform = MatrixTranslation(Vec3(128,50,0)+shadowOffset),
      material = 
      {
          color = shadowColor
      }
    }
  },
  dcl.rg:Draw
  {
    mesh = dcl.font:Text
    {
      font = verattf,
      text = "Drop tasklet directory to get started!",
      size =  fontSize,
      transform = MatrixTranslation(Vec3(128,50,0)),
      material = 
      {
          color = fontColor
      }
    }
  }
}

