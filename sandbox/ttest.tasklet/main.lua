
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
      items={
        {
          title="File", 
          menu={ -- menu constructor 
            items=
            {
              {title="m1 First"},
              {title="m1 Second"},
              "-",
              {title="m1 Third"},              
              {title="m1 Fourth"},              
            }
          }
        },
        {
          title="Edit", 
          menu={
            items=
            {
              {
                title="m2 First",
                menu=
                {
                  items=
                  {
                   {title="sm1 First"},
                   {title="sm1 Second"},
                   {
                     title="sm1 Third",
                     menu=
                     {
                       items=
                       {
                         {title="sm11 First"},
                         {title="sm11 Second"},                       
                         {title="sm11 Third"},                       
                       }
                     }
                   },              
                   {
                     title="sm1 Fourth",
                     menu=
                     {
                       items=
                       {
                         {title="sm12 First"},
                         {title="sm12 Second"},                       
                       }
                     }                     
                   },              
                   {title="sm1 Fifth"},
                  }
                }                             
              },
              {title="m2 Second"},
              "-",
              {title="m2 Third"},              
              {title="m2 Fourth"},              
              "-",
              {title="m2 Fifth"},              
            },
          }
        },
        {
          title="Long Menu Name", 
          menu={
            items=
            {
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
        },
        {
          title="Short Again", 
          menu={
            items=
            {
              {title="m4 First"},
              {title="m4 Second"},
              {title="m4 Third"},              
            }
          }
        },
      }
    },
  }
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
