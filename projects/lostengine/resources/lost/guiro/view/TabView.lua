module("lost.guiro.view", package.seeall)

require("lost.guiro.view.View")

lost.common.Class "lost.guiro.view.TabView" "lost.guiro.view.View" {}

-- mandatory, set by theme:
-- self.tabbarView
-- self.contentView
-- params:
-- * items: table containing tables containing string/view pairs
--     e.g. items = {{"first view", v1}, {"second view",v2}}
function TabView:constructor(args)
  -- remove items from args so they don't clash with tabbar 
  local t = args or {}
  local items = t.items or {}
  t.items = nil
  lost.guiro.view.View.constructor(self, t)
  self.id = t.id or "tabview"
  self.tabbarItems = {}
  self.tabViews = {}
  for k,item in ipairs(items) do
    table.insert(self.tabbarItems, item[1])
    table.insert(self.tabViews, item[2])    
  end
  self.tabbarView._items = self.tabbarItems
  self.tabbarView:rebuildButtons()
  for k,view in ipairs(self.tabViews) do
    self:addSubview(view)
  end
  self.selected = t.selected or 1
  self:select(self.selected)
  self.tabbarView:addEventListener("tabBarSelectionChanged",function(event) self:tabbarSelectionChanged(event) end)
  self.contentView:clip(t.clip or true)
end

function TabView:tabbarSelectionChanged(event)
  self:select(event.target.selected)
end

function TabView:addSubview(view,forceSubview)
  if self.contentView and not forceSubview then
    self.contentView:addSubview(view)
  else
    lost.guiro.view.View.addSubview(self, view)
  end
end

function TabView:hideAll()
  for k,v in ipairs(self.tabViews) do
    v:hidden(true)
  end
end

function TabView:select(val)
  self:hideAll()
  self.tabViews[val]:hidden(false)
  self.selected = val

  local ev = lost.guiro.event.Event("tabViewSelectionChanged")
  ev.bubbles = true
  ev.target = self
  self:dispatchEvent(ev)  
end
