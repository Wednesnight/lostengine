require("lost.guiro.view.UserInterface")
require("lost.guiro.layer.Rect")
require("lost.guiro.layer.RoundedRect")
require("lost.guiro.layer.Text")

local Color = lost.common.Color

function startup()
  local ui = lost.guiro.ui()
  
  local r = lost.guiro.layer.Rect
  local rr = lost.guiro.layer.RoundedRect
  local t = lost.guiro.layer.Text
  
  ui:add
  {
    lost.guiro.view.View
    {
      id = "anotherView",
      bounds = {0,0,"1","1"},
      listeners =
      {
        drop = function (event)
          tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(event.filename))
        end
      },
      sublayers = 
      {
        r{bounds={0,0,"1","1"},id="whiteBackground",filled=true,color=Color(1,1,1)},
        rr{id="leftBox",bounds={{"left",10}, {"bottom",10}, {".5",-15},{"1",-20}},color=Color(1,.5,.2),radius=8,filled=true},
        rr{id="rightBox",bounds={{"right",-10}, {"bottom",10}, {".5",-15},{"1",-20}},color=Color(1,.2,.2),radius=8,filled=true},
        rr{id="centerTextBg",bounds={"center", "center", ".7", ".4"},color=Color(1, 1, 1,1),radius=8,filled=true},
        t{id="dropMessage",bounds={"center", "center", ".6", ".4"},font={"Grinched", 28},text="Drop tasklet directory here!",color=Color(0,0,0)},
        rr{id="leftCutout",bounds={"left", "center", ".10", ".4"},color=Color(1, 1, 1),radius=8,filled=true},
        rr{id="rightCutout",bounds={"right", "center", ".10", ".4"},color=Color(1, 1, 1),radius=8,filled=true},
      }
    }
  }
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)
end

function keyHandler(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
