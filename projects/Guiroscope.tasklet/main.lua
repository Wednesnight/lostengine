require("lost.declarative.Context")
require("lost.guiro.view.Label")
require("lost.guiro.view.TabBar")
require("lost.guiro.layer.Text")
require("lost.guiro.layer.Rect")
require("lost.guiro.layout.Horizontal")
require("lost.guiro.layout.Vertical")

local Color = lost.common.Color

function startup()
  local tabbarheight = 52
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  lost.guiro.themeManager() -- make sure it is instantiated and gradients are created
  local layers = require("layers")
  layers:hidden(false)
  local views = require("views")
  views:hidden(true)

  ui = lost.guiro.view.UserInterface
  {
    listeners = 
    {
        tabBarSelectionChanged = function(event) 
            if event.target.id ~= "main" then return end
            if event.target.selected == 1 then
              layers:hidden(false)
              views:hidden(true)
            else
              layers:hidden(true)
              views:hidden(false)          
            end
          end
    },
    subviews = 
    {
      lost.guiro.view.View
      {
          id="mainToolBar",
          style="toolbar", bounds={0,"top","1",tabbarheight},
          subviews=
          {
            lost.guiro.view.Label{bounds={10,0,"1",50},halign="left", text="Guiroscope",font={"Grinched",30}},
            lost.guiro.view.TabBar{id="main",bounds={"right", "top", "1","1"},items={"Layers", "Views"},selected = 1,size="regular"},
          }
      },
      lost.guiro.view.View
      {
          id="mainView",
          bounds = {0,0,"1",{"1",-tabbarheight}},
          clip=true,
          subviews = {layers,views}
      }
    }
  }
--  ui.layer:print()
--  ui:printSubviews()
end

function update()
--  ui.layer.renderNode:print()
--  local u = gcinfo();collectgarbage("collect");local u2 = gcinfo();log.debug("used: "..u.."kb after gc: "..u2.."kb")
end

function shutdown()
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
