local Color = lost.common.Color
local tabbarHeight = 30

local layers_rect = require("layers_rect")
local layers_roundedrect = require("layers_roundedrect")
local layers_text = require("layers_text")
local layers_hline = require("layers_hline")
local layers_disc = require("layers_disc")

local allViews = {layers_rect, layers_roundedrect, layers_text, layers_hline, layers_disc}

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
layers_rect:hidden(false)

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
      items={"Rect", "RoundedRect", "Text", "HLine", "Disc"},
      selected = 1
      },
    lost.guiro.view.View{
      bounds={0,0,"1",{"1",-tabbarHeight}},
      subviews=allViews
    }
  }
}