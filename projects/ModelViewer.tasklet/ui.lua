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

local controller = require("controller")
local scene = require("scene")

return dcl.guiro:Screen
{
  listeners =
  {
    keyDown = controller.keyHandler
  },
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = Bounds(xleft(), ybottom(), wfit(), hfit()),
    dcl.guiro:Window
    {
      id = "window",
      bounds = Bounds(xleft(), ybottom(), wfit(), hfit()),
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
          text = "Model Viewer"
        }
      },
      dcl.guiro:View
      {
        id = "renderView",
        bounds = Bounds(xleft({abs = 10}), ybottom({abs = 10}), wrel(1, -20), hrel(1, -50)),
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
            mouseUp = controller.sceneMouseUp,
            drop = controller.droppedModel
          }
        }
      }
    }
  }
}
