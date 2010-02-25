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

local controller = require("controller")

return dcl.guiro:View
{
  id = "toolView",
  bounds = Bounds(xright({abs = -10}), ytop({abs = -40}), wabs(350), hrel(1, -50)),
  dcl.guiro:View
  {
    id = "shaderInfo",
    bounds = Bounds(xleft(), ytop(), wfit(), hrel(1, -40)),
    showFrame = true,
    dcl.guiro:Label
    {
      id = "shaderLabel",
      bounds = Bounds(xleft(), ytop(), wrel(1), habs(50)),
      fontSize = 16,
      text = "(Drop shader here)",
      showFrame = true,
      listeners =
      {
        dragEnter = controller.dragEnter,
        dragUpdate = controller.dragUpdate,
        dragLeave = controller.dragLeave,
        drop = controller.droppedShader
      }
    },
    dcl.guiro:VBox
    {
      id = "shaderParams",
      bounds = Bounds(xleft({abs = 2}), ytop({abs = -50}), wrel(1, -4), hrel(1, -50)),
      halign = "left",
      mode = "stack",
      spacing = 5
    }
  },
  dcl.guiro:Button
  {
    id = "reloadButton",
    bounds = Bounds(xleft(), ybottom({abs = 5}), wfit(), habs(25)),
    title = "Reload Shader",
    listeners =
    {
      click = controller.reloadShader
    }
  }
}
