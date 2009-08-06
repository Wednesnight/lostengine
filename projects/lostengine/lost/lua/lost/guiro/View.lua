-- lost.guiro.View
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.Bounds")
require("lost.guiro.event.EventDispatcher")

lost.common.Class "lost.guiro.View"
{

  -- helper for auto-generated view ids
  indices = {},

  isView = true,
	parent = nil,

  focusable = false,
  hidden = false,
  mouseInside = false,
  receivesEvents = true,
--  sendsEvents = true ????

  dirtyLayout = true,
  dirtyChildLayout = false,

  -- geometry cache
  currentGlobalRect = lost.math.Rect(),
  currentLocalRect = lost.math.Rect(),

  dirty = true,
  focused = false
}

function View:createDefaultId()
  local result = nil
  local name = self:className()
  if (not self.indices[name]) then
    self.indices[name] = 1
  else
    self.indices[name] = self.indices[name]+1
  end
  result = name .. self.indices[name]
  return result
end

--[[ 
    constructor
  ]]
function View:constructor()
  -- all views and derived classes receive a default id in the
  -- constructor so they can be uniquely identified
  self.id = self:createDefaultId()

  -- RenderGraph node
  self.rootNode = lost.rg.Node.create()
  self.rootNode.name = self.id
  self.renderNode = lost.rg.Node.create()
  self.renderNode.name = "renderNode"
  self.childNodes = lost.rg.Node.create()
  self.childNodes.name = "childNodes"
  self.rootNode:add(self.renderNode)
  self.rootNode:add(self.childNodes)

  -- mesh container
  self.meshes = {}

  self.bounds = Bounds(xabs(0), yabs(0), wabs(0), habs(0))
  self.subviews = {}

  -- setup event dispatchers
  self.defaultEventDispatcher = lost.guiro.event.EventDispatcher()
  self.captureEventDispatcher = lost.guiro.event.EventDispatcher()
  self.currentGlobalRect = lost.math.Rect()
end

function View:__tostring()
  return "View: "..self.id
end

function View:__eq(other)
  return rawequal(self, other)
end

--[[ 
    inserts child into self.subviews 
  ]]
function View:addSubview(newview, pos)
	log.debug("trying to add "..newview.id)
  if (pos == nil) then
    pos = table.maxn(self.subviews) + 1
  end
  if (newview.id) then
    if (self(newview.id) == nil) then
      table.insert(self.subviews, pos, newview)
      self.childNodes:add(newview.rootNode)
      newview:setParent(self)
    else
      log.error("subview '".. newview.id .."' already exists")
			error("subview already exists")
    end
  else
    log.error("cannot append subview without id")
  end
end

--[[ 
    removes child from self.subviews and sets child.parent to nil
  ]]
function View:removeSubview(subview)
  local idx = 1
  for k,view in next,self.subviews do
    if rawequal(view, subview) then
      table.remove(self.subviews, idx)
      self.childNodes:remove(subview.rootNode)
      subview:setParent(nil)
      break
    end
    idx = idx+1
  end
end

--[[
    returns subview with given subviewId
    nil if subviewId is invalid
  ]]
function View:__call(subviewId)
  local result = nil
  for k,view in next,self.subviews do
    if (view.id == subviewId) then
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

--[[
    checks if point is within view rect
  ]]
function View:containsCoord(point)
  local globalRect = self:globalRect()
  return globalRect:contains(point)
end

--[[
    updates own and subviews's layout
  ]]
function View:updateLayout(forceUpdate)
  -- view needs update
  if forceUpdate or self.dirtyLayout then
    self.dirtyLayout = false
    self.dirtyChildLayout = false
    if self.parent then
      log.debug("fetching parents globalRect")
      local pgr = self.parent:globalRect()
      log.debug("pgr "..pgr.x.." "..pgr.y.." "..pgr.width.." "..pgr.height)
      self.currentGlobalRect = self.bounds:rect(pgr)
    else
      self.currentGlobalRect = self.bounds:rect(lost.math.Rect())
    end
    self.currentLocalRect = self.bounds:rect(lost.math.Rect())

    for key,view in next,self.subviews do
      view:updateLayout(true)
    end

  -- child needs update
  elseif self.dirtyChildLayout then
    self.dirtyChildLayout = false
    for key,view in next,self.subviews do
      if view.dirtyLayout or view.dirtyChildLayout then
        view:updateLayout()
      end
    end
  end
  
--  return lost.math.Rect(self.currentGlobalRect), lost.math.Rect(self.currentLocalRect)
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
    sets dirty flag to trigger render
  ]]
function View:needsRedraw()
  self.dirty = true
end

function View:update()
  self:updateLayout()
  if dirty then
    self.dirty = false
  end
  for k,view in next,self.subviews do
    view:update()
  end
end

function View:screen()
  if self.parent then
    return self.parent:screen()
  else
    return nil
  end
end

--[[ 
    prints self.subviews hierarchy
  ]]
function View:printSubviews(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in next,self.subviews do
    view:printSubviews(prefix .."    ")
  end
end

--[[
    the following three functions exist in order to hide some View implementation
    details from the EventManager and to prettify the calling code in the EventManager.
    If the structure proves to be viable, we can remove these methods and call the
    appropriate dispatchers directly from the EventManager.
  ]]
-- !!!!!!!!! DO NOT CALL DIRECTLY, ONLY USED BY EVENTMANAGER
function View:dispatchCaptureEvent(event)
  self.captureEventDispatcher:dispatchEvent(event)
end
function View:dispatchTargetEvent(event)
--  log.debug(self.id..": targetting")
  self.defaultEventDispatcher:dispatchEvent(event)
end
-- called by EventManager, don't call directly
function View:dispatchBubbleEvent(event)
  self.defaultEventDispatcher:dispatchEvent(event)
end
-- ^^^^^^^^^ DO NOT CALL DIRECTLY, ONLY USED BY EVENTMANAGER
