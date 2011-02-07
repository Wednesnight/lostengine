
local Color=lost.common.Color
local Vec2=lost.math.Vec2

local f = {"Vera",12}

function startup()
  require("lost.guiro")

  lost.guiro.ui():addEventListener("menuItemSelected",function(event) 
        log.debug(event.type.." "..event.target.id)
        for k,v in ipairs(event.indexPath) do
          log.debug(v)
        end
        local mi = event.target:menuItemForIndexPath(event.indexPath)
        mi:checked(not mi:checked())
      end
      )

  lost.guiro.ui():add
  {
    lost.guiro.view.Label{bounds={0,{"top",-40},"1",50},
                          text="Supercalifragilistic",
                          font={"Grinched", 48},
                          color=Color(1,0,0)},
    lost.guiro.view.MenuBar
    {
      bounds={0,"top","1",30},
      items=require("menudef"),
      size="regular",
    },
    lost.guiro.view.MenuBar
    {
      bounds={0,{"top",-50},"1",30},
      items=require("menudef"),
      size="small",
    },
    lost.guiro.view.MenuBar
    {
      bounds={0,{"top",-100},"1",30},
      items=require("menudef"),
      size="mini",
    }
  }
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
