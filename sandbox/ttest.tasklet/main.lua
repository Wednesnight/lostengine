
local Color=lost.common.Color
local Vec2=lost.math.Vec2

local f = {"Vera",12}

function startup()
  require("lost.guiro")

  lost.guiro.ui():add
  {
    lost.guiro.view.Label{bounds={0,{"top",-40},"1",50},
                          text="Supercalifragilistic",
                          font={"Grinched", 48},
                          color=Color(1,0,0)},
    lost.guiro.view.MenuBar
    {
      bounds={0,"top","1",30},
      items={
        {
          title="Menu 1", 
          items={
            {title="m1 First"},
            {title="m1 Second"},
            "-",
            {title="m1 Third"},              
            {title="m1 Fourth"},              
          }
        },
        {
          title="Menu 2", 
          items={
            {title="m2 First"},
            {title="m2 Second"},
            "-",
            {title="m2 Third"},              
            {title="m2 Fourth"},              
            "-",
            {title="m2 Fifth"},              
          }
        },
        {
          title="Menu 3", 
          items={
            {title="m3 First"},
            {title="m3 Second"},
            "-",
            {title="m3 Third"},              
            {title="m3 Fourth"},              
            "-",
            {title="m3 Fifth"},              
            {title="m3 Sixth"},              
            {title="m3 Seventh"},              
          }
        }
      }
    },
  }
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
