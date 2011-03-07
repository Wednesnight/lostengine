svc_cellsize=50
svc_cellspacing=4
svc_gridWidth=4*svc_cellsize+3*svc_cellspacing
svc_gridHeight=5*svc_cellsize+4*svc_cellspacing

local Vec2 = lost.math.Vec2

local svc_margin = 0
local svc_blockSpacing = 2

function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      bounds={svc_margin,svc_margin,{"1",-2*svc_margin},{"1",-2*svc_margin}},
      subviews=
      {
        lost.guiro.view.ScrollView
        {
          size="regular",
          bounds={"left",0,{".33333",-svc_blockSpacing},"1"},
          contentSize=Vec2(svc_gridWidth,svc_gridHeight),
          hasVerticalScrollbar = true,
          hasHorizontalScrollbar = true,          
          subviews=
          {
            require("grid")(),            
          }
        },
        lost.guiro.view.ScrollView
        {
          size="small",
          bounds={"center",0,{".33333",-svc_blockSpacing},"1"},
          contentSize=Vec2(svc_gridWidth,svc_gridHeight),
          hasVerticalScrollbar = true,
          hasHorizontalScrollbar = false,          
          subviews=
          {
            require("grid")(),            
          }
        },
        lost.guiro.view.ScrollView
        {
          size="mini",
          bounds={"right",0,{".33333",-svc_blockSpacing},"1"},
          contentSize=Vec2(svc_gridWidth,svc_gridHeight),
          hasVerticalScrollbar = false,
          hasHorizontalScrollbar = true,          
          subviews=
          {
            require("grid")(),            
          }
        }
      }
    }
  }
end

function update()
--  lost.guiro.ui().layer:print()
--  lost.guiro.ui():printSubviews()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
