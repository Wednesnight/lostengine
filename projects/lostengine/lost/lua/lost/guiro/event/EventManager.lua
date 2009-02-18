module("lost.guiro.event", package.seeall)

require("lost.guiro.event.MouseEvent")

class "lost.guiro.event.EventManager"
EventManager = _G["lost.guiro.event.EventManager"]

function pack(...)
  return arg
end

--- main entry point for low level keyboard and mouse events that are received from the application
-- the EventManager will correctly distribute the events inside the view hierarchy.
-- Event distribution is a two step process:
--  * propagate the event through the view tree
--  * dispatch the event at the current view to the registered listeners
-- 
--
--
function EventManager:__init()
  log.debug("EventManager:__init")
  self.previousMouseMoveStack = nil
  self.previousMouseClickStack = nil
end


function EventManager:findViewStack(rootView, mouseEvent)
  local pos = mouseEvent.pos
  -- FIXME maybe move viewStack to self since there can be only one and theres no poit recreating this table on every event?
  local viewStack = {} -- this will hold the stack of views for the different event phases 
  local containsPoint  = rootView:containsCoord(pos)-- containsPoint designates if a view contained the point
  local view = rootView
  local k, v
  while containsPoint and (not view.hidden) and view.receivesEvents do
    viewStack[#viewStack+1] = view
    containsPoint = false
    local i = #(view.children)
    local v = nil
    while i > 0 do
      v = view.children[i]
--      log.debug(" --- "..v.id)
      if v:containsCoord(pos) then
        containsPoint = true;
        view = v
        break
      end
      i = i - 1
    end
  end
  return viewStack
end

function EventManager:logViewStack(vs)
  local k = nil
  local v = nil

  for k,v in pairs(viewStack) do
    log.debug(k.." - "..v.id)
  end  
end

-- calls dispatchCaptureEvent on all views in the stack from index [1,n)
function EventManager:dispatchCaptureEvents(viewStack, event, targetIndex)
  if viewStack and (targetIndex>1) and event then 
    local maxv = targetIndex - 1
    local k = nil
    local v = nil
    for k,v in ipairs(viewStack) do
      if (k <= maxv) and (not event.stopDispatch) then
--        log.debug("calling capture on "..v.id)
          event.currentTarget = v
          v:dispatchCaptureEvent(event)
      else
        break
      end
    end
  end
end

-- calls dispatchTargetEvent on view n
function EventManager:dispatchTargetEvent(viewStack, event, targetIndex)
  if viewStack and (targetIndex>=1) and event and (not event.stopDispatch)then
    local currentView = viewStack[targetIndex]
--    log.debug("calling target on "..currentView.id)
    currentView:dispatchTargetEvent(event)
  end
end

-- calls dispatchBubbleEvent on views [1, n) in reverse order
function EventManager:dispatchBubbleEvents(viewStack, event, targetIndex)
  if viewStack and (targetIndex>1) and event then
    local i = targetIndex-1
    while (i > 0) and (not event.stopDispatch) do
      local currentView = viewStack[i]
--      log.debug("calling bubble on "..currentView.id)
      event.currentTarget = currentView
      currentView:dispatchBubbleEvent(event)
      i = i -1 
    end
  end
end

--[[
function findTargetIndex(viewStack, event)
  -- event must contain a valid and unique target view id
  -- find index of target view in viewStack
  local targetIndex = -1
  for k,v in ipairs(viewStack) do
    if v.id == event.target then
      targetIndex = k
      break
    end
  end

  -- bail if targetIndex could not be found
  if targetIndex == -1 then
    error("couldn't find index for targetView "..event.target, 1)
  end
end
]]

function EventManager:propagateEvent(viewStack, event, targetIndex)
--  log.debug("propagate "..event.target.id.." "..event.type)
  if not event.stopPropagation then self:dispatchCaptureEvents(viewStack, event, targetIndex) end
  if not event.stopPropagation then self:dispatchTargetEvent(viewStack, event, targetIndex) end
  if not event.stopPropagation then self:dispatchBubbleEvents(viewStack, event, targetIndex) end
end


function EventManager:propagateEnterLeaveEvents(viewStack, event)
  -- no previous stack means the each view is the target for an enter event
  if not self.previousMouseMoveStack then 
    self.previousMouseMoveStack = viewStack
    for k,v in ipairs(viewStack) do
      event.target = viewStack[k]
      event.type = lost.guiro.event.MouseEvent.MOUSE_ENTER
      self:propagateEvent(viewStack, enterEvent, k)
    end
  else -- leave events for old views, enter events for new views
    -- find the index from where the viewstacks differ
    -- build a safe maximum for the initial search  
    local maxi = math.max(#viewStack, #(self.previousMouseMoveStack))
    local oldView = nil
    local newView = nil
    local i = 1
    while i <= maxi do
      if i <= #self.previousMouseMoveStack then
        oldView = self.previousMouseMoveStack[i]
      else
        oldView = nil
      end
      if i <= #viewStack then
        newView = viewStack[i]
      else
        newView = nil
      end  
      if oldView ~= newView then
        if oldView then
          event.target = oldView
          event.type = lost.guiro.event.MouseEvent.MOUSE_LEAVE
          self:propagateEvent(self.previousMouseMoveStack, event, i)
        end
        if newView then
          event.target = newView
          event.type = lost.guiro.event.MouseEvent.MOUSE_ENTER
          self:propagateEvent(viewStack, event, i)
        end
      end
      i = i + 1
    end
    self.previousMouseMoveStack = viewStack    
  end
end

-- propagates an event down the tree of views starting from (and including) the given rootView
-- expects the incoming event type to be lost.application.MouseEvent 
-- wraps it to lst.guiro.MouseEvent
function EventManager:propagateMouseEvent(rootView, event)
--  log.debug("propagateEvent: " .. event.type)
  local mouseevent = lost.guiro.event.MouseEvent(event) 
  viewStack = self:findViewStack(rootView, mouseevent)

  mouseevent.currentTarget = nil -- will be set to the receiving view upon dispatch
  mouseevent.target = viewStack[#viewStack] -- the lowermost view is the target of the click 

  self:propagateEvent(viewStack, mouseevent, #viewStack)

  if mouseevent.type == lost.guiro.event.MouseEvent.MOUSE_MOVE then
    self:propagateEnterLeaveEvents(viewStack, mouseevent)
  end  
  
  return viewStack
end
