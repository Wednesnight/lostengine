using "lost.common.Color"

local controller = require("controller")
local toolview = require("toolview")
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
          text = "Shader Lab"
        }
      },
      dcl.guiro:View
      {
        id = "renderView",
        bounds = {{"left", 10}, {"bottom", 10}, {"1", -380}, {"1", -50}}, 
        showFrame = true,
        dcl.guiro:RenderView
        {
          id = "scene",
          bounds = {{"left", 1}, {"bottom", 1}, {"1", -2}, {"1", -2}}, 
          rendergraph = scene,
          listeners =
          {
            mouseDown = controller.sceneMouseDown,
            mouseMove = controller.sceneMouseMove,
            mouseUp = controller.sceneMouseUp
          }
        }
      },
      toolview
    }
  }
}
