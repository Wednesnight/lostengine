module("lost.guiro.event", package.seeall)

require("lost.guiro.event.MouseEvent")

class "lost.guiro.event.EventManager"
EventManager = _G["lost.guiro.event.EventManager"]


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
    for k,v in pairs(view.children) do
      if v:containsCoord(pos) then
        containsPoint = true;
        view = v
        break
      end
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
function EventManager:dispatchCaptureEvents(viewStack, event)
  if viewStack and ((#viewStack)>1) and event then 
    local maxv = #viewStack - 1
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
function EventManager:dispatchTargetEvent(viewStack, event)
  if viewStack and ((#viewStack)>=1) and event and (not event.stopDispatch)then
    local currentView = viewStack[#viewStack]
--    log.debug("calling target on "..currentView.id)
    currentView:dispatchTargetEvent(event)
  end
end

-- calls dispatchBubbleEvent on views [1, n) in reverse order
function EventManager:dispatchBubbleEvents(viewStack, event)
  if viewStack and ((#viewStack)>1) and event then
    local i = (#viewStack)-1
    while (i > 0) and (not event.stopDispatch) do
      local currentView = viewStack[i]
--      log.debug("calling bubble on "..currentView.id)
      event.currentTarget = currentView
      currentView:dispatchBubbleEvent(event)
      i = i -1 
    end
  end
end

-- propagates an event down the tree of views starting from (and including) the given rootView
-- expects the incoming event type to be lost.application.MouseEvent 
-- wraps it to lst.guiro.MouseEvent
function EventManager:propagateMouseEvent(rootView, event)
--  log.debug("propagateEvent: " .. event.type)
  local mouseevent = lost.guiro.event.MouseEvent(event) 
  viewStack = self:findViewStack(rootView, mouseevent)
  -- reset target and currentTarget
  mouseevent.currentTarget = nil -- will be set to the receiving view upon dispatch
  mouseevent.target = viewStack[#viewStack] -- the lowermost view is the target of the click 
--  self:logViewStack(viewStack)
  if not mouseevent.stopPropagation then self:dispatchCaptureEvents(viewStack, mouseevent) end
  if not mouseevent.stopPropagation then self:dispatchTargetEvent(viewStack, mouseevent) end
  if not mouseevent.stopPropagation then self:dispatchBubbleEvents(viewStack, mouseevent) end
end
