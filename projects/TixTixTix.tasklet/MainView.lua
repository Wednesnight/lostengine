require("lost.common.Class")
require("lost.guiro.Screen")
require("lost.declarative.Context")

lost.common.Class "MainView" {}

using "lost.declarative.Context"
using "lost.guiro.Bounds"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.xrel"
using "lost.guiro.yrel"
using "lost.guiro.xleft"
using "lost.guiro.ytop"
using "lost.guiro.xright"
using "lost.guiro.ybottom"
using "lost.guiro.xcenter"
using "lost.guiro.ycenter"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.guiro.wabs"
using "lost.guiro.habs"
using "lost.guiro.wfit"
using "lost.guiro.hfit"
using "lost.common.Color"

function MainView:constructor(tasklet)

  -- init declarative context
  local dcl = Context(tasklet.loader)

  self.screen = dcl.guiro:Screen
  {
    dcl.guiro:UserInterface
    {
      id = "ui",
      bounds = Bounds(xcenter(), ycenter(), wabs(250), habs(185)),
      dcl.guiro:Window
      {
        id = "window",
        bounds = Bounds(xleft(), ybottom(), wfit(), hfit()),
        showFrame = false,
        showBackground = true,
        backgroundColor = Color(.1, .1, .1, 1),
        dcl.guiro:View
        {
          id = "logo",
          bounds = Bounds(xleft(), ytop(), wfit(), habs(40)),
          dcl.guiro:Label
          {
            bounds = Bounds(xleft(), ytop(), wrel(1), habs(25)),
            fontSize = 12,
            halign = "center",
            text = "LostEngine"
          },
          dcl.guiro:Label
          {
            bounds = Bounds(xleft(), ytop({abs = -15}), wrel(1), habs(25)),
            fontSize = 16,
            halign = "center",
            text = "TixTixTix"
          }
        },
        dcl.guiro:View
        {
          id = "menu",
          bounds = Bounds(xleft({abs = 10}), ybottom({abs = 10}), wrel(1, -20), hrel(1, -50)),
          showFrame = true,
          dcl.guiro:VBox
          {
            id = "buttons",
            bounds = Bounds(xabs(10), yabs(10), wrel(1, -20), hrel(1, -20)),
            halign = "center",
            mode = "stack",
            spacing = 5,
            dcl.guiro:Button
            {
              id = "newGameButton",
              bounds = Bounds(xleft(), ytop(), wrel(1), habs(25)),
              title = "New Game"
            },
            dcl.guiro:Button
            {
              id = "optionsButton",
              bounds = Bounds(xleft(), ytop(), wrel(1), habs(25)),
              title = "Options",
              enabled = false
            },
            dcl.guiro:Button
            {
              id = "helpButton",
              bounds = Bounds(xleft(), ytop(), wrel(1), habs(25)),
              title = "Help",
              enabled = false
            },
            dcl.guiro:Button
            {
              id = "exitButton",
              bounds = Bounds(xleft(), ytop(), wrel(1), habs(25)),
              title = "Exit"
            }
          }
        }
      }
    }
  }
  
  -- initially hidden
--  self.screen:hidden(true)
end
