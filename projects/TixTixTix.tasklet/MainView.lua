require("lost.common.Class")
require("lost.guiro.view.Screen")
require("lost.declarative.Context")

lost.common.Class "MainView" {}

using "lost.declarative.Context"
using "lost.common.Color"

function MainView:constructor(tasklet)

  -- init declarative context
  local dcl = Context(tasklet.loader)

  self.screen = dcl.guiro:Screen
  {
    dcl.guiro:UserInterface
    {
      id = "ui",
      bounds = {"center", "center", 250, 135},
      dcl.guiro:Window
      {
        id = "window",
        bounds = {"left", "bottom", "1", "1"},
        showFrame = false,
        showBackground = true,
        backgroundColor = Color(.1, .1, .1, 1),
        dcl.guiro:View
        {
          id = "logo",
          bounds = {"left", "top", "1", 40},
          dcl.guiro:Label
          {
            bounds = {"left", "top", "1", 25},
            fontSize = 12,
            halign = "center",
            text = "LostEngine"
          },
          dcl.guiro:Label
          {
            bounds = {"left", {"top", -15}, "1", 25},
            fontSize = 16,
            halign = "center",
            text = "TixTixTix"
          }
        },
        dcl.guiro:View
        {
          id = "menu",
          bounds = {{"left", 10}, {"bottom", 10}, {"1", -20}, {"1", -50}},
          showFrame = true,
          dcl.guiro:VBox
          {
            id = "buttons",
            bounds = {10, 10, {"1", -20}, {"1", -20}},
            halign = "center",
            mode = "stack",
            spacing = 5,
            dcl.guiro:Button
            {
              id = "startButton",
              bounds = {"left", "top", "1", 25},
              title = "Start"
            },
            dcl.guiro:Button
            {
              id = "exitButton",
              bounds = {"left", "top", "1", 25}, 
              title = "Exit"
            }
          }
        }
      }
    }
  }
end
