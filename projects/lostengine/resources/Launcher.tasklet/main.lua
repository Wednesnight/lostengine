require("lost.guiro.view.UserInterface")
require("lost.guiro.layer.Rect")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.layer.Text")

local Color = lost.common.Color

function drop(event)
  tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(event.filename))
end

function startup()
  local ui = lost.guiro.ui()
  ui:add
  {
    lost.guiro.view.View
    {
      id = "anotherView",
      bounds = {0,0,"1","1"},
      sublayers = 
      {
        lost.guiro.layer.Rect
        {
          bounds = {0,0,"1","1"},
          id = "whiteBackground",
          filled = true,
          color = Color(1,1,1)
        },
        lost.guiro.layer.RoundedRect
        {
          id = "leftBox",
          bounds = {{"left",10}, {"bottom",10}, {".5",-15},{"1",-20}},
          color = Color(1,.5,.2),
          radius = 8,
          filled = true
        },
        lost.guiro.layer.RoundedRect
        {
          id = "rightBox",
          bounds = {{"right",-10}, {"bottom",10}, {".5",-15},{"1",-20}},
          color = Color(1,.2,.2),
          radius = 8,
          filled = true
        },
        lost.guiro.layer.RoundedRect
        {
          id = "centerTextBg",
          bounds = {"center", "center", ".6", ".4"},
          color = Color(1, 1, 1,1),
          radius = 8,
          filled = true
        },
--[[        lost.guiro.layer.RoundedRect
        {
          id = "centerTextFrame",
          bounds = {"center", "center", ".6", 60},
          color = Color(0, 0, 0, 1),
          radius = 8,
          filled = false,
          width = 1
        },]]
        lost.guiro.layer.Text
        {
          id="dropMessage",
          bounds = {"center", "center", ".6", ".4"},
          font = {"Vera", 14},
          text = "Drop tasklet directory here!",
          color = Color(0,0,0)          
        },

        lost.guiro.layer.RoundedRect
        {
          id = "leftCutout",
          bounds = {"left", "center", ".15", ".4"},
          color = Color(1, 1, 1),
          radius = 8,
          filled = true,
        },
        lost.guiro.layer.RoundedRect
        {
          id = "rightCutout",
          bounds = {"right", "center", ".15", ".4"},
          color = Color(1, 1, 1),
          radius = 8,
          filled = true,
        },

      }
    }
  }
  ui:addEventListener("drop", drop)
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
  return true
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.window:close()
  end
end
