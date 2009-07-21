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
    viewport = Rect(0,0,screenSize.width,screenSize.height)
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
      text = "Drop tasklet directory to get started!",
      size =  12,
      transform = MatrixTranslation(Vec3(200,50,0))
    }
  }
}
