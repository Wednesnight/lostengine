local controller = require("controller")

return dcl.guiro:View
{
  id = "toolView",
  bounds = {{"right", -10}, {"top", -40}, 350, {"1", -50}},
  dcl.guiro:View
  {
    id = "shaderInfo",
    bounds = {"left", "top", "1", {"1", -40}},
    showFrame = true,
    dcl.guiro:Label
    {
      id = "shaderLabel",
      bounds = {"left", "top", "1", 50},
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
      bounds = {{"left", 2}, {"top", -50}, {"1", -4}, {"1", -50}}, 
      halign = "left",
      mode = "stack",
      spacing = 5
    }
  },
  dcl.guiro:Button
  {
    id = "reloadButton",
    bounds = {"left", {"bottom", 5}, "1", 25},
    title = "Reload Shader",
    listeners =
    {
      click = controller.reloadShader
    }
  }
}
