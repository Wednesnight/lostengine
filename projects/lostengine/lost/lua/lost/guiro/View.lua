module("lost.guiro", package.seeall)

--[[
     View class
  ]]
class "lost.guiro.View" (lost.event.EventDispatcher)
View = _G["lost.guiro.View"]

function View:__init() super()
  log.debug("View:__init()")
  self.children = {}
  self.isView = true
end

function View:appendChild(child)
  table.insert(self.children, child)
end

function View:render()
  log.debug("View:render()")
  for k,view in next,self.children do
    view:render()
  end
end

function View:__finalize()
  log.debug("View:__finalize()")
end
