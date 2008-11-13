module("lost.guiro", package.seeall)

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
    constructor
  ]]
function View:__init() super()
  self.bounds = lost.math.Rect(0,0,0,0)
  self.children = {}
  self.isView = true
  self.listeners = {}
end

--[[ 
    inserts child into self.children and sets child.parent to self
  ]]
function View:appendChild(child)
  if (child.id) then
    if (not self.children[child.id]) then
      self.children[child.id] = child
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
  for k,v in next,self.children do
    if (v == child) then
      table.remove(self.children, idx)
      child.setParent(nil)
      break
    end
    idx = idx+1
  end
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
    uses self.renderer and self.style to create visual representation
  ]]
function View:render(context)
  if (self.renderer and self.style) then
    self.renderer:render(context, self, self.style)
  end
  for k,view in next,self.children do
    view:render(context)
  end
end

--[[
    returns the Views rect in absolute (screen) coordinates
  ]]
function View:globalRect()
  local result = lost.math.Rect(self.bounds)
  if self.parent then
    local parentRect = self.parent:globalRect()
    result.x = result.x + parentRect.x
    result.y = result.y + parentRect.y
  end
  return result
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
