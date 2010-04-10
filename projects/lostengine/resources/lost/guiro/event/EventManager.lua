module("lost.guiro.event", package.seeall)

require("lost.common.Class")
require("lost.guiro.event.Event")
require("lost.guiro.event.MouseEvent")
require("lost.guiro.event.FocusEvent")
require("lost.guiro.event.KeyEvent")
require("lost.guiro.event.TouchEvent")
require("lost.guiro.event.DragNDropEvent")

lost.common.Class "lost.guiro.event.EventManager" {}

using "lost.guiro.event.Event"

--- main entry point for low level keyboard and mouse events that are received from the application
-- the EventManager will correctly distribute the events inside the view hierarchy.
-- Event distribution is a two step process:
--  * propagate the event through the view tree
--  * dispatch the event at the current view to the registered listeners
-- 
--
--
function EventManager:constructor(rootView)
  self.rootView = rootView
  self.previousMouseMoveStack = {}
  self.previousMouseClickStack = {}
  self.previousDragUpdateStack = {}
  self.previousFocusStack = { self.rootView }
  self.focusChanged = false
  self.currentFocusedView = self.rootView
end


function EventManager:findViewStack(rootView, mouseEvent)
  local pos = mouseEvent.pos
  -- FIXME maybe move viewStack to self since there can be only one and theres no poit recreating this table on every event?
  local viewStack = {} -- this will hold the stack of views for the different event phases 
  local containsPoint  = rootView:containsCoord(pos)-- containsPoint designates if a view contained the point
  local view = rootView
  local k, v
  while containsPoint and view:isDerivedFrom("lost.guiro.HasEvents") do
    viewStack[#viewStack+1] = view
    containsPoint = false
    local i = #(view.subviews)
    local v = nil
    while i > 0 do
      v = view.subviews[i]
--      log.debug(" --- "..v.id)
      if v:containsCoord(pos) then
--        log.debug("contains")
        containsPoint = true;
        view = v
        break
      else
--        log.debug("o noes")
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

-- calls routeEvent with phase capture on all views in the stack from index [1,n]
function EventManager:dispatchCaptureEvents(viewStack, event, targetIndex)
  if viewStack and (targetIndex > 1) and event then 
    local maxv = targetIndex - 1
    local k = nil
    local v = nil
    for k,v in ipairs(viewStack) do
      if (k <= maxv) and (not event.stopDispatch) then
--        log.debug("calling capture on "..v.id)
        event.currentTarget = v
        event.phase = Event.PHASE_CAPTURE
        v:routeEvent(event)
      else
        break
      end
    end
  end
end

-- calls routeEvent with phase target on view n
function EventManager:dispatchTargetEvent(viewStack, event, targetIndex)
  if viewStack and (targetIndex >= 1) and event and (not event.stopDispatch)then
    local currentView = viewStack[targetIndex]
--    log.debug("calling target on "..currentView.id)
    event.currentTarget = event.target
    event.phase = Event.PHASE_TARGET
    currentView:routeEvent(event)
  end
end

-- calls routeEvent with phase bubble on views [1, n) in reverse order
function EventManager:dispatchBubbleEvents(viewStack, event, targetIndex)
  if viewStack and (targetIndex > 1) and event then
    local i = targetIndex-1
    while (i > 0) and (not event.stopDispatch) do
      local currentView = viewStack[i]
--      log.debug("calling bubble on "..currentView.id)
      event.currentTarget = currentView
      event.phase = Event.PHASE_BUBBLE
      currentView:routeEvent(event)
      i = i -1 
    end
  end
end

function EventManager:propagateEvent(viewStack, event, targetIndex)
--  log.debug("propagate "..event.target.id.." "..event.type)
  if not event.stopPropagation then self:dispatchCaptureEvents(viewStack, event, targetIndex) end
  if not event.stopPropagation then self:dispatchTargetEvent(viewStack, event, targetIndex) end
  if not event.stopPropagation then self:dispatchBubbleEvents(viewStack, event, targetIndex) end
end


function EventManager:propagateEnterLeaveEvents(viewStack, oldViewStack, event, typeEnter, typeLeave)
  -- leave events for old views, enter events for new views
  -- iterate over both stacks, send appropriate events to old or new views
--  log.debug(#viewStack ..", ".. #oldViewStack)
  local maxi = math.max(#viewStack, #oldViewStack)
  local oldView = nil
  local newView = nil
  local i = 1
  while i <= maxi do
    oldView = oldViewStack[i]
    newView = viewStack[i]
    if oldView ~= newView then
      if oldView then
        event.target = oldView
        event.type = typeLeave
--        log.debug("leave: ".. tostring(oldView))
        self:propagateEvent(oldViewStack, event, i)
      end
      if newView then
        event.target = newView
        event.type = typeEnter
--        log.debug("enter: ".. tostring(newView))
        self:propagateEvent(viewStack, event, i)
      end
    end
    i = i + 1
  end
end

function EventManager:propagateUpDownEvents(viewStack, event)
  if event.type == lost.guiro.event.MouseEvent.MOUSE_DOWN then
    event.target = viewStack[#viewStack] -- the lowermost view is the target
    self:propagateEvent(viewStack, event, #viewStack)
    self.previousMouseClickStack = viewStack
  elseif event.type == lost.guiro.event.MouseEvent.MOUSE_UP then
    local idx = math.max(#viewStack, #(self.previousMouseClickStack))
    local oldView = self.previousMouseClickStack[idx] -- the lowermost view is the target
    local newView = viewStack[idx] -- the lowermost view is the target
    if oldView and oldView ~= newView then
      event.target = oldView
      event.type = lost.guiro.event.MouseEvent.MOUSE_UP_OUTSIDE
      self:propagateEvent(self.previousMouseClickStack, event, idx)
      event.type = lost.guiro.event.MouseEvent.MOUSE_UP
      self:propagateEvent(self.previousMouseClickStack, event, idx)
    end
    if newView then
      event.target = newView
      event.type = lost.guiro.event.MouseEvent.MOUSE_UP_INSIDE
      self:propagateEvent(viewStack, event, idx)
      event.type = lost.guiro.event.MouseEvent.MOUSE_UP
      self:propagateEvent(viewStack, event, idx)
    end
    self.previousMouseClickStack = {}
  end
end

function EventManager:propagateFocusEvents(viewStack, event)
  if event.type == lost.guiro.event.MouseEvent.MOUSE_DOWN then
    local focusEvent = lost.guiro.event.FocusEvent()
    local maxi = math.max(#viewStack, #(self.previousFocusStack))
    local oldView = nil
    local newView = nil
    local i = 1
    while i <= maxi do
      oldView = self.previousFocusStack[i]
      newView = viewStack[i]
      if oldView ~= newView then
        self.focusChanged = true
        if oldView and oldView.focusable then
          oldView.focused = false
          focusEvent.target = oldView
          focusEvent.type = lost.guiro.event.FocusEvent.FOCUS_LOST
          self:propagateEvent(self.previousFocusStack, focusEvent, i)
        end
        if newView and newView.focusable then
          newView.focused = true
          focusEvent.target = newView
          focusEvent.type = lost.guiro.event.FocusEvent.FOCUS_RECEIVED
          self:propagateEvent(viewStack, focusEvent, i)
        end
      end
      i = i + 1
    end
    self.previousFocusStack = viewStack
  end
end


-- propagates an event down the tree of views starting from (and including) the given rootView
-- expects the incoming event type to be lost.application.MouseEvent 
-- wraps it to lost.guiro.event.MouseEvent
function EventManager:propagateMouseEvent(rootView, event)
--  log.debug("propagateEvent: " .. event.type)
  local mouseevent = lost.guiro.event.MouseEvent(event) 
  local viewStack = self:findViewStack(rootView, mouseevent)

  -- dispatch up, down
  if (mouseevent.type == lost.guiro.event.MouseEvent.MOUSE_UP) or (mouseevent.type == lost.guiro.event.MouseEvent.MOUSE_DOWN) then
    self:propagateUpDownEvents(viewStack, mouseevent)
    if (mouseevent.type == lost.guiro.event.MouseEvent.MOUSE_DOWN) then
      self:propagateFocusEvents(viewStack, mouseevent)
    end  
  -- scroll, move
  else
    mouseevent.target = viewStack[#viewStack] -- the lowermost view is the target
    self:propagateEvent(viewStack, mouseevent, #viewStack)
  end
  
  -- dispatch enter, leave
  if mouseevent.type == lost.guiro.event.MouseEvent.MOUSE_MOVE then
    self:propagateEnterLeaveEvents(viewStack, self.previousMouseMoveStack, mouseevent,
      lost.guiro.event.MouseEvent.MOUSE_ENTER, lost.guiro.event.MouseEvent.MOUSE_LEAVE)
    self.previousMouseMoveStack = viewStack
  end  

  return viewStack
end

-- propagates an event using the hovered View
-- expects the incoming event type to be lost.application.DragNDropEvent
-- wraps it to lost.guiro.event.DragNDropEvent
function EventManager:propagateDragNDropEvent(rootView, event)
  local dropEvent = lost.guiro.event.DragNDropEvent(event)
  local viewStack = self:findViewStack(rootView, dropEvent)

  -- dispatch dragNdrop
  dropEvent.target = viewStack[#viewStack]
  self:propagateEvent(viewStack, dropEvent, #viewStack)

  -- dispatch enter, leave
  if dropEvent.type == lost.guiro.event.DragNDropEvent.DRAG_UPDATE then
    self:propagateEnterLeaveEvents(viewStack, self.previousDragUpdateStack, dropEvent,
      lost.guiro.event.DragNDropEvent.DRAG_ENTER, lost.guiro.event.DragNDropEvent.DRAG_LEAVE)
    self.previousDragUpdateStack = viewStack
  end
end


-- propagates a key event down the tree of views to the currently focused view
-- expects the incoming event type to be lost.application.KeyEvent
-- wraps it to lost.guiro.event.KeyEvent
function EventManager:propagateKeyEvent(event)
--  log.debug("propagateEvent: " .. event.type)
  local keyevent = lost.guiro.event.KeyEvent(event) 

--  keyevent.currentTarget = nil -- will be set to the receiving view upon dispatch
  keyevent.target = self:focusedView() -- the currently focused view is the target of the click
  self:propagateEvent(self.previousFocusStack, keyevent, #self.previousFocusStack)
end

function EventManager:propagateTouchEvent(rootView, event)
  local touchEvent = lost.guiro.event.TouchEvent(event)

  log.debug("received ".. touchEvent.size .." touches")
  for touch in touchEvent.touches do
    log.debug(tostring(touch.location))
  end
end

function EventManager:focusedView()
  if self.focusChanged then
    local idx = #self.previousFocusStack
    while idx > 0 do
      if self.previousFocusStack[idx].focusable then
        self.currentFocusedView = self.previousFocusStack[idx]
        break
      end
      idx = idx - 1
    end
    self.focusChanged = false
  end
  return self.currentFocusedView
end