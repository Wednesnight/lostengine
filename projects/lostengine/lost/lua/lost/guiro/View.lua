module("lost.guiro", package.seeall) -- View

require("lost.common.Class")
require("lost.common.Object")
require("lost.guiro.Bounds")
require("lost.guiro.event.EventDispatcher")

--[[
     View class
  ]]
View = lost.common.Class("lost.guiro.View", lost.common.Object)

--[[ 
    constructor
  ]]
function View:__init() lost.common.Object.__init(self)
  self.bounds = Bounds(xabs(0), yabs(0), wabs(0), habs(0))
  self.children = {}
  self.isView = true
	self.parent = nil
  self.hidden = false
  self.receivesEvents = true
  self.mouseInside = false
--  self.sendsEvents = true ????

  -- layout state flags
  self.dirtyLayout = true
  self.dirtyChildLayout = false
  self.currentGlobalRect = lost.math.Rect()
  self.currentLocalRect = lost.math.Rect()

  -- render state
  self.dirty = true
  self.theme = lost.guiro.config.theme
  self.renderer = self.theme.renderers[self:className()]()
  self.style = self.theme.styles[self:className()]()

  -- setup event dispatchers
  self.defaultEventDispatcher = lost.guiro.event.EventDispatcher()
  self.captureEventDispatcher = lost.guiro.event.EventDispatcher()  
end

function View:__tostring()
  return "View: "..self.id
end

function View:__eq(other)
  return rawequal(self, other)
end

--[[ 
    inserts child into self.children 
  ]]
function View:appendChild(child, pos)
	log.debug("trying to add "..child.id)
  if (pos == nil) then
    pos = table.maxn(self.children) + 1
  end
  if (child.id) then
    if (self(child.id) == nil) then
      table.insert(self.children, pos, child)
      child:setParent(self)
    else
      log.error("child '".. child.id .."' already exists")
			error("child already exists")
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
    if rawequal(view, child) then
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
    basically same functionality as EventDispatcher:addEventListener.
    if capture is true, the listener is only added to the capture event listeners
    if capture is false, the listener is only added to the default listeners (target and bubble)
  ]]
function View:addEventListener(which, listener, capture)
--  log.debug(tostring(self.id)..": adding listener for "..which)
  if capture then 
    self.captureEventDispatcher:addEventListener(which, listener)
  else
    self.defaultEventDispatcher:addEventListener(which, listener)
  end
end

-- calling dispatchEvent on a view notifies all target listeners and bubbles the event if event.bubbles is enabled
function View:dispatchEvent(event)
  if event then
    if not event.stopDispatch then
      self:dispatchTargetEvent(event) 
    end
    if event.bubbles and (not event.stopPropagation) then
      self:bubbleEvent(event)
    end
  end
end

-- helper function to bubble an event up the parent view hierarchy
function View:bubbleEvent(event)
  local currentView = self.parent
  while currentView and (not event.stopPropagation) do
    currentView:dispatchBubbleEvent(event)
    currentView = currentView.parent
  end
end

function View:removeEventListener(which, listener, capture)
  if capture then
    self.captureEventDispatcher:removeEventListener(which, listener)
  else
    self.defaultEventDispatcher:removeEventListener(which, listener)
  end
end

-- called by EventManager, don't call directly
function View:dispatchCaptureEvent(event)
  self.captureEventDispatcher:dispatchEvent(event)
end

-- called by EventManager, don't call directly
function View:dispatchTargetEvent(event)
--  log.debug(self.id..": targetting")
  self.defaultEventDispatcher:dispatchEvent(event)
end

-- called by EventManager, don't call directly
function View:dispatchBubbleEvent(event)
  self.defaultEventDispatcher:dispatchEvent(event)
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
    update method for render preparation
  ]]
function View:update(canvas)
  for key,view in next,self.children do
    view:update(canvas)
  end
end

--[[
    triggers render on children
  ]]
function View:render(canvas, forceRender)
  local globalRect = self:updateLayout()
  if forceRender or self.dirty then
    self.dirty = false

    if self.renderer then
      self.renderer:render(canvas, self, self.style)
    end

    for k,view in next,self.children do
      view:render(canvas, true)
    end
  end
end

--[[
    sets dirty flag to trigger render
  ]]
function View:needsRedraw()
  self.dirty = true
end


--[[ 
    helper methods
  ]]

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

function View:screen()
  if self.parent then
    return self.parent:screen()
  else
    return nil
  end
end
