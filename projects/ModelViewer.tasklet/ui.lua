using "lost.common.Color"

local controller = require("controller")
local scene = require("scene")
local shadowScene = require("shadowScene")

controller.debugView = dcl.guiro:RenderView
{
  id = "debugScene",
  bounds = {{"left", 1}, {"bottom", 1}, {".5", -2}, {"1", -2}}, 
  rendergraph = scene,
  listeners =
  {
    mouseDown = controller.sceneMouseDown,
    mouseMove = controller.sceneMouseMove,
    mouseUp = controller.sceneMouseUp,
    drop = controller.droppedModel
  }
}
controller.debugView:texture(controller.debugView._framebuffer:depthTexture())

controller.renderView = dcl.guiro:RenderView
{
  id = "scene",
  bounds = {{"right",1}, {"bottom", 1}, {".5", -2}, {"1", -2}},
  rendergraph = shadowScene,
  listeners =
  {
    mouseDown = controller.sceneMouseDown,
    mouseMove = controller.sceneMouseMove,
    mouseUp = controller.sceneMouseUp,
    drop = controller.droppedModel
  }
}

return dcl.guiro:Screen
{
  listeners =
  {
    keyDown = controller.keyHandler
  },
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = {"left", "bottom", "1", "1"},
    dcl.guiro:Window
    {
      id = "window",
      bounds = {"left", "bottom", "1", "1"},
      showFrame = false,
      showBackground = true,
      backgroundColor = Color(.1, .1, .1, 1),
      dcl.guiro:View
      {
        bounds = {"left", "top", "1", 40},
        dcl.guiro:Label
        {
          bounds = {10, "top", 100, 25},
          fontSize = 12,
          halign="left",
          text = "LostEngine"
        },
        dcl.guiro:Label
        {
          bounds = {15, {"top", -15}, 100, 25}, 
          fontSize = 16,
          halign="left",
          text = "Model Viewer"
        }
      },
      dcl.guiro:View
      {
        id = "renderView",
        bounds = {{"left", 10}, {"bottom", 10}, {"1", -20}, {"1", -50}},
        showFrame = true,
        controller.debugView,
        controller.renderView
      }
    }
  }
}
