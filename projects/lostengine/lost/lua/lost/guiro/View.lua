module("lost.guiro", package.seeall)

--[[
     View class
  ]]
class "lost.guiro.View" (lost.event.EventDispatcher)
View = _G["lost.guiro.View"]

View.bases = { "View" }
View.addBase = function(parent, class, name)
  class.bases = {}
  for k,v in next,parent.bases do
    class.bases[k] = v
  end
  class.addBase = parent.addBase
  table.insert(class.bases, name)
end

function View:__init() super()
  log.debug("View:__init()")
  self.children = {}
  self.isView = true
end

function View:appendChild(child)
  table.insert(self.children, child)
end

function View:dispatchEvent(event)
  log.debug("View:dispatchEvent()")
  for k,view in next,self.children do
    view:dispatchEvent(event)
  end
end

function View:render()
--  log.debug("View:render()")
  for k,view in next,self.children do
    view:render()
  end
end

function View:whoAmI()
  local prefix = ""
  for k,v in next,self.bases do
    print(prefix, k, v)
    prefix = prefix .."  "
  end
end

function View:__finalize()
  log.debug("View:__finalize()")
end
