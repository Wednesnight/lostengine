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

local scene = require("scene")
local controller = require("controller")

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
        bounds = Bounds(xleft({abs = 10}), ybottom({abs = 10}), wrel(1, -330), hrel(1, -50)),
        showFrame = true,
        dcl.guiro:RenderView
        {
          id = "scene",
          bounds = Bounds(xleft({abs = 1}), ybottom({abs = 1}), wrel(1, -2), hrel(1, -2)),
          rendergraph = scene,
          listeners =
          {
            mouseDown = controller.sceneMouseDown,
            mouseMove = controller.sceneMouseMove,
            mouseUp = controller.sceneMouseUp
          }
        }
      },
      dcl.guiro:VBox
      {
        id = "toolView",
        bounds = Bounds(xright({abs = -10}), ybottom({abs = 40}), wabs(300), hrel(1, -80)),
        showFrame = true,
        halign = "left",
        mode = "stack",
        spacing = 5,
        dcl.guiro:Label
        {
          bounds = Bounds(xabs(0), yabs(0), wrel(1), habs(20)),
          fontSize = 12,
          text = "Shader",
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
            droppedFile = controller.droppedShader
          }
        },
        dcl.guiro:VBox
        {
          id = "shaderParams",
          bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1, -75)),
          showFrame = true,
          halign = "left",
          mode = "stack",
          spacing = 5
        }
      },
      dcl.guiro:Button
      {
        id = "updateButton",
        bounds = Bounds(xright({abs = -10}), ybottom({abs = 10}), wabs(300), habs(25)),
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
