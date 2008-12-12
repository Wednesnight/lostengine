module("lost.guiro", package.seeall)

require("lost.guiro.Bounds")

--[[
     View class
  ]]
class "lost.guiro.View" (lost.event.EventDispatcher)
View = _G["lost.guiro.View"]

--[[ 
    class (not instance) based table holding the list of class names
    must be extended through View:addBase()
  ]]
View.bases = { "View" }

--[[ 
    constructor
  ]]
function View:__init() super()
  self.bounds = Bounds(xabs(0), yabs(0), wabs(0), habs(0))
  self.children = {}
  self.isView = true
  self.listeners = {}

  -- layout state flags
  self.dirtyLayout = true
  self.dirtyChildLayout = false
  self.currentGlobalRect = lost.math.Rect()
  self.currentLocalRect = lost.math.Rect()

  -- render state flags
  self.dirty = true
end

--[[ 
    adds className to class.bases
    a derived class MUST use its direct base to call this method because class.bases is extended from self.bases
  ]]
function View:addBase(class, className)
  class.bases = {}
  for k,v in next,self.bases do
    class.bases[k] = v
  end
  class.addBase = self.addBase
  table.insert(class.bases, className)
end

--[[ 
    checks if self is a or derived from className
  ]]
function View:is(className)
  local result = self.bases
  if result then
    for k,base in next,self.bases do
      result = base == className
      if result then
        break
      end
    end
  end
  return result
end

--[[ 
    returns the last entry in self.bases
  ]]
function View:className()
  local result = "nil"
  if self.bases then
    result = self.bases[table.maxn(self.bases)]
  end
  return result
end

--[[ 
    inserts child into self.children and sets child.parent to self
  ]]
function View:appendChild(child)
  if (child.id) then
    if (self(child.id) == nil) then
      table.insert(self.children, child)
      child:setParent(self)
    else
      log.error("child '".. child.id .."' already exists")
    end
  else
    log.error("cannot append child without id")
  end
end

--[[ 
    removes child from self.children and sets child.parent to nil
  ]]
function View:removeChild(child)
  local idx = 1
  for k,view in next,self.children do
    if (view == child) then
      table.remove(self.children, idx)
      child:setParent(nil)
      break
    end
    idx = idx+1
  end
end

--[[
    returns child with given childId
    nil if childId is invalid
  ]]
function View:__call(childId)
  local result = nil
  for k,view in next,self.children do
    if (view.id == childId) then
      result = view
      break
    end
  end
  return result
end

--[[ 
    sets the parent
  ]]
function View:setParent(parent)
  self.parent = parent
  if (self.parent) then
    --
  else
    --
  end
end

--[[ 
    overrides lost.event.EventDispatcher.addEventListener to preserve lua event types
  ]]
function View:addEventListener(which, listener)
  if not self.listeners[which] then
    self.listeners[which] = {}
  end
  table.insert(self.listeners[which], listener)
end

--[[ 
    derived from EventDispatcher, routes all events to self.children
  ]]
function View:dispatchEvent(event)
  --[[
      first we iterate through the "lua" event handlers
    ]]
  if self.listeners[event.type] then
    for k,listener in next,self.listeners[event.type] do
      listener(event)
    end
  end

  --[[
      then the native ones
    ]]
  lost.event.EventDispatcher.dispatchEvent(self, event)

  --[[
      and now the children
    ]]
  for k,view in next,self.children do
    view:dispatchEvent(event)
  end
end

--[[
    gets topmost child at given point
  ]]
function View:getViewAt(point)
  local result = nil
  if self.parent then
    result = self.parent:getViewAt(point)
  end
  if (result == nil or rawequal(result, self)) then
    local globalRect = self:globalRect()
    if globalRect:contains(point) then
      local idx = table.maxn(self.children)
      while idx > 0 do
        local childRect = self.children[idx]:globalRect()
        if childRect:contains(point) then
          result = self.children[idx]
          break
        end
        idx = idx-1
      end
    end
  end
  return result
end

--[[
    checks if point is within view rect
  ]]
function View:containsCoord(point)
  local globalRect = self:globalRect()
  return globalRect:contains(point)
end

--[[
    updates own and children's layout
  ]]
function View:updateLayout(forceUpdate)
  -- view needs update
  if forceUpdate or self.dirtyLayout then
    self.dirtyLayout = false
    self.dirtyChildLayout = false
    if self.parent then
      self.currentGlobalRect = self.bounds:rect(self.parent:globalRect())
    else
      self.currentGlobalRect = self.bounds:rect(lost.math.Rect())
    end
    self.currentLocalRect = self.bounds:rect(lost.math.Rect())
    for key,view in next,self.children do
      view:updateLayout(true)
    end

  -- child needs update
  elseif self.dirtyChildLayout then
    self.dirtyChildLayout = false
    for key,view in next,self.children do
      if view.dirtyLayout or view.dirtyChildLayout then
        view:updateLayout()
      end
    end
  end
  
  return self.currentGlobalRect, self.currentLocalRect
end

--[[
    returns the Views rect in absolute (screen) coordinates
  ]]
function View:globalRect()
  local globalRect, localRect = self:updateLayout()
  return globalRect
end

--[[
    returns the Views rect in relative (parent) coordinates
  ]]
function View:localRect()
  local globalRect, localRect = self:updateLayout()
  return localRect
end

--[[
    sets dirtyLayout and parent's dirtyChildLayout flags
  ]]
function View:needsLayout()
  self:needsRedraw()
  if not self.dirtyLayout then
    self.dirtyLayout = true
    if self.parent then
      self.parent:needsChildLayout()
    end
  end
end

--[[
    sets dirtyChildLayout flag
  ]]
function View:needsChildLayout()
  if not self.dirtyChildLayout then
    self.dirtyChildLayout = true
    if self.parent then
      self.parent:needsChildLayout()
    end
  end
end

--[[
    resize and call needsLayout
  ]]
function View:resize(bounds)
  self.bounds = bounds
  self:needsLayout()
end

--[[
    redraw prototype
  ]]
function View:redraw(context)
  -- empty
end

--[[
    update method for render preparation
  ]]
function View:update(context)
  for key,view in next,self.children do
    view:update(context)
  end
end

--[[
    triggers redraw on children
  ]]
function View:render(context, forceRender)
  self:updateLayout()
  if forceRender or self.dirty then
    self.dirty = false
    self:redraw(context)

    for k,view in next,self.children do
      view:render(context, true)
    end
  end
end

--[[
    sets dirty flag to trigger redraw
  ]]
function View:needsRedraw()
  self.dirty = true
  if self.parent then
    self.parent.dirty = true
  end
end


--[[ 
    helper methods
  ]]


--[[ 
    prints self.bases hierarchy
  ]]
function View:printBases()
  local prefix = ""
  for k,v in next,self.bases do
    log.debug(prefix .."|-- ".. v)
    prefix = prefix .."    "
  end
end

--[[ 
    prints self.children hierarchy
  ]]
function View:printChildren(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in next,self.children do
    view:printChildren(prefix .."    ")
  end
end
