using "lost.common.Color"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"

camPos = Vec3(1,2,2)

return
{
  dcl.rg:ClearColor
  {
    color = Color(0,0,0)
  },
  dcl.rg:Clear
  {
    mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
  },
  dcl.rg:Camera3D
  {
    viewport = Rect(0, 0, 320, 240),
    position = camPos,
    fovY = 45.0,
    depth = Vec2(1.0, 1000.0),
    target = Vec3(0, 0, 0),
    stickToTarget = true
  },
  dcl.rg:DepthTest
  {
    true
  },
  dcl.rg:Draw
  {
    mesh = dcl.mesh:Obj
    {
      filename = "magnolia_tri.obj",
      material =
      {
        shader = dcl.gl:Shader
        {
          filename = "light",
          params =
          {
            LightPosition = camPos,
            LightColor = Color(1, 1, 1),
            EyePosition = camPos,
            Specular = Color(.75, .75, .5),
            Ambient = Color(.1, .1, .1),
            Kd = 0.8,
            Scale = Vec2(0.7, 3.7),
            Threshold = Vec2(.3, .2),
            SurfaceColor = Color(1,1,1)
          }
        }
      }
    }
  }
}
