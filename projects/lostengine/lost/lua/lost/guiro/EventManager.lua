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
--    print("VIEW "..view.id.." "..#viewStack)
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

-- propagates an event down the tree of views starting from (and including) the given rootView
function EventManager:propagateMouseEvent(rootView, event)
  log.debug("propagateEvent: " .. event.type)
  event = lost.application.MouseEvent.cast(event) -- we need to cast the event otherwise luabind won't recognise it correctly
  viewStack = self:findViewStack(rootView, event)
  
  for k,v in pairs(viewStack) do
    log.debug(k.." - "..v.id)
  end
  
end