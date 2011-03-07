module("lost.guiro.view", package.seeall)

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
  self.tabbarView._items = self.tabbarItems
  for k,item in ipairs(items) do
    self:addItem(item[1], item[2])
  end
  self:select(t.selected or 1)
  self.tabbarView:addEventListener("tabBarSelectionChanged",function(event) self:tabbarSelectionChanged(event) end)
  local clip = true
  if t.clip ~= nil then
    clip = t.clip
  end
  self.contentView:clip(clip)
	self._bbcounter = lost.profiler.BBCount("lost.guiro.view.TabView")          
end

function TabView:tabbarSelectionChanged(event)
  if not self.ignoreEvents then
    self:select(event.target.selected)
  end
end

function TabView:addItem(name, view)
  table.insert(self.tabbarItems, name)
  table.insert(self.tabViews, view)    
  self.tabbarView:rebuildButtons()
  self:addSubview(view)
end

function TabView:removeItem(nameOrId)
  local index = 0
  for k,v in ipairs(self.tabViews) do
    if v.id == nameOrId then
      index = k
      break
    end
  end
  if index == 0 then
    for k,v in ipairs(self.tabbarItems) do
      if v == nameOrId then
        index = k
        break
      end
    end
  end
  if index > 0 then
    table.remove(self.tabbarItems, index)
    self:removeSubview(table.remove(self.tabViews, index))
    self.tabbarView:rebuildButtons()
    if index == self.selected and #self.tabbarItems > 0 then
      self:select(math.min(index+1, #self.tabbarItems), true)
    end
  end
end

function TabView:addSubview(view,forceSubview)
  if self.contentView and not forceSubview then
    self.contentView:addSubview(view)
  else
    lost.guiro.view.View.addSubview(self, view)
  end
end

function TabView:removeSubview(view,forceSubview)
  if self.contentView and not forceSubview then
    self.contentView:removeSubview(view)
  else
    lost.guiro.view.View.removeSubview(self, view)
  end
end

function TabView:hideAll()
  for k,v in ipairs(self.tabViews) do
    v:hidden(true)
  end
end

function TabView:select(val, force)
  local selected = 0
  if type(val) == "string" then
    local found = false
    for k,v in ipairs(self.tabViews) do
      if v.id == val then
        found = true
        selected = k
        break
      end
    end
    if not found then
      for k,v in ipairs(self.tabbarItems) do
        if v == val then
          found = true
          selected = k
          break
        end
      end
    end
    if not found then
      return false
    end
  else
    selected = val
  end

  local result = selected > 0 and selected <= #self.tabViews
  if result and (selected ~= self.selected or force) then
    self:hideAll()
    self.tabViews[selected]:hidden(false)
    self.selected = selected
    self.ignoreEvents = true
    self.tabbarView:select(self.selected, force)
    self.ignoreEvents = false

    local ev = lost.guiro.event.Event("tabViewSelectionChanged")
    ev.bubbles = true
    ev.target = self
    self:dispatchEvent(ev)
  end

  return result
end
