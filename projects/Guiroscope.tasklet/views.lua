local tabbarHeight = 30

local allViews = {
  require("views_view"),
  require("views_label"),
  require("views_button"),
  require("views_tabbar"),
  require("views_image"),
  require("views_window"),
}

local function hideAll()
  for k,v in ipairs(allViews) do
    v:hidden(true)
  end
end

local function selectionChanged(event)
  if event.target.id ~= "viewselect" then return end
  hideAll()
  allViews[event.target.selected]:hidden(false)
end

hideAll()
allViews[1]:hidden(false)


return lost.guiro.view.View
{
  listeners = {tabBarSelectionChanged = selectionChanged},
  id="views",
  bounds = {0,0,"1","1"},
  subviews=
  {
    lost.guiro.view.TabBar
    {
      id="viewselect",
      bounds={"left", "top", "1",tabbarHeight},
      items={"View", "Label","Button", "TabBar", "Image", "Window"}
      },
    lost.guiro.view.View
    {
      bounds = {0,0,"1",{"1",-tabbarHeight}},
      subviews = allViews
    }
  }
}
