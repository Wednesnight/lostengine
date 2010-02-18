using "lost.guiro.Bounds"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.xrel"
using "lost.guiro.yrel"
using "lost.guiro.ytop"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.guiro.wabs"
using "lost.guiro.habs"
using "lost.common.Color"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.resource.Loader"
using "lost.resource.FilesystemRepository"
using "lost.resource.ApplicationResourceRepository"
using "lost.application.K_ESCAPE"

local shaderLoader = Loader.create()
shaderLoader:addRepository(FilesystemRepository.create("/"))
shaderLoader:addRepository(ApplicationResourceRepository.create())

local camPos = Vec3(1,2,2)

return dcl.guiro:Screen
{
  listeners =
  {
    keyDown = function(event)
      if event.key == K_ESCAPE then
        running = false
      end
    end,

    droppedFile = function(event)
      local self = event.target:rootView()
--      self("ui")("window")("label"):text(event.filename)
    end
  },
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      id = "window",
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      showFrame = false,
      showBackground = true,
      backgroundColor = Color(.1, .1, .1, 1),
      dcl.guiro:RenderView
      {
        id = "renderView",
        bounds = Bounds(xrel(.25), yrel(.25), wrel(.5), hrel(.5)),
        rendergraph =
        {
          dcl.rg:ClearColor
          {
            color = Color(1,0,0)
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
      },
      dcl.guiro:Label
      {
        id = "label",
        bounds = Bounds(xrel(.25), ytop(), wrel(.5), hrel(.25)),
        text = "RenderView with magnolia model"
      }
    }
  }
}
