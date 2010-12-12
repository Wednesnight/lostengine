local Color = lost.common.Color
local tabbarHeight = 30

local allViews = {require("layers_rect"), 
                  require("layers_roundedrect"),
                  require("layers_text"),
                  require("layers_hline"),
                  require("layers_disc"),
                  require("layers_image"),                  
                  }

local function hideAll()
  for k,v in ipairs(allViews) do
    v:hidden(true)
  end
end

local function selectionChanged(event)
  hideAll()
  allViews[event.target.selected]:hidden(false)
end

hideAll()
allViews[1]:hidden(false)

return lost.guiro.view.View
{
  listeners = {tabBarSelectionChanged = selectionChanged},
  id="layers",
  bounds = {0,0,"1","1"},
  subviews =
  {
    lost.guiro.view.TabBar{
      id="layersTabBar",
      bounds={"left", "top", "1",tabbarHeight},
      items={"Rect", "RoundedRect", "Text", "HLine", "Disc", "Image"},
      selected = 1
      },
    lost.guiro.view.View{
      bounds={0,0,"1",{"1",-tabbarHeight}},
      subviews=allViews
    }
  }
}