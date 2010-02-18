using "lost.guiro.Bounds"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.xrel"
using "lost.guiro.yrel"
using "lost.guiro.xleft"
using "lost.guiro.ytop"
using "lost.guiro.xright"
using "lost.guiro.ybottom"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.guiro.wabs"
using "lost.guiro.habs"
using "lost.guiro.wfit"
using "lost.guiro.hfit"
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
      dcl.guiro:View
      {
        bounds = Bounds(xleft(), ytop(), wfit(), habs(40)),
        dcl.guiro:Label
        {
          bounds = Bounds(xabs(10), ytop(), wabs(100), habs(25)),
          fontSize = 12,
          halign="left",
          text = "LostEngine"
        },
        dcl.guiro:Label
        {
          bounds = Bounds(xabs(15), ytop({abs = -15}), wabs(100), habs(25)),
          fontSize = 16,
          halign="left",
          text = "Shader Lab"
        }
      },
      dcl.guiro:View
      {
        id = "renderView",
        bounds = Bounds(xleft({abs = 10}), ybottom({abs = 10}), wrel(1, -260), hrel(1, -50)),
        showFrame = true,
        dcl.guiro:RenderView
        {
          id = "scene",
          bounds = Bounds(xleft({abs = 1}), ybottom({abs = 1}), wrel(1, -2), hrel(1, -2)),
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
        }
      },
      dcl.guiro:VBox
      {
        id = "toolView",
        bounds = Bounds(xright({abs = -10}), ybottom({abs = 10}), wabs(230), hrel(1, -50)),
        showFrame = true,
        halign = "left",
        mode = "stack",
        spacing = 5,
        dcl.guiro:Label
        {
          bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(15)),
          fontSize = 12,
          text = "Shader:",
          halign = "left"
        },
        dcl.guiro:Label
        {
          id = "shaderLabel",
          bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(50)),
          fontSize = 16,
          text = "(Drop shader here)",
          showFrame = true,
          listeners =
          {
            droppedFile = function(event)
              if event.currentTarget == event.target then
                local relativeName = string.match(event.filename, "([^/%.]*)%.[^/%.]*$")
                if relativeName ~= nil then
                  event.target:text(relativeName)
                end
              end
            end
          }
        },
        dcl.guiro:Button
        {
          id = "updateButton",
          bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(25)),
          title = "Update",
          listeners =
          {
            click = function(event)
              if event.currentTarget == event.target then
                --
              end
            end
          }
        }
      }
    }
  }
}
