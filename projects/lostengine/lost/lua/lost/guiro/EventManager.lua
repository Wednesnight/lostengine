module("lost.guiro", package.seeall)


class "lost.guiro.EventManager"
EventManager = _G["lost.guiro.EventManager"]

function EventManager:__init()
  log.debug("EventManager:__init")
end

function EventManager:findViewStack(rootView, mouseEvent)
  local pos = mouseEvent.pos
  local viewStack = {} -- this will hold the stack of views for the different event phases 
  local containsPoint  = rootView:containsCoord(pos)-- containsPoint designates if a view contained the point
  local view = rootView
  local k, v
  while containsPoint do
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
--    log.debug(k.." - "..v.id)
  end  
end

-- calls dispatchCaptureEvent on all views in the stack from index [1,n)
function EventManager:dispatchCaptureEvents(viewStack, event)
  if viewStack and ((#viewStack)>1) and event then 
    local maxv = #viewStack - 1
    local k = nil
    local v = nil
    for k,v in ipairs(viewStack) do
      if k <= maxv then
        --log.debug("calling capture on "..v.id)
        v:dispatchCaptureEvent(event)
      else
        break
      end
    end
  end
end

-- calls dispatchTargetEvent on view n
function EventManager:dispatchTargetEvent(viewStack, event)
  if viewStack and ((#viewStack)>=1) and event then
    --log.debug("calling target on "..viewStack[#viewStack].id)
    viewStack[#viewStack]:dispatchTargetEvent(event)
  end
end

-- calls dispatchBubbleEvent on views [1, n) in reverse order
function EventManager:dispatchBubbleEvents(viewStack, event)
  if viewStack and ((#viewStack)>1) and event then
    local i = (#viewStack)-1
    local k = nil
    local v = nil
    while i > 0 do
      --log.debug("calling bubble on "..viewStack[i].id)
      viewStack[i]:dispatchBubbleEvent(event)
      i = i -1 
    end
  end
end

-- propagates an event down the tree of views starting from (and including) the given rootView
function EventManager:propagateMouseEvent(rootView, event)
  --log.debug("propagateEvent: " .. event.type)
-- we need to cast the event to mouseevent or we won'T be able to access the pos member to find the view stack. This is LAME!
  local mouseevent = lost.application.MouseEvent.cast(event) 
  viewStack = self:findViewStack(rootView, mouseevent)
  self:logViewStack(viewStack)
  self:dispatchCaptureEvents(viewStack, event)
  self:dispatchTargetEvent(viewStack, event)
  self:dispatchBubbleEvents(viewStack, event)
end

