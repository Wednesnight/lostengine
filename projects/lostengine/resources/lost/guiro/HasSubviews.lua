--[[
    lost.guiro.HasSubviews
    
    HasSubviews is an interface that implements all view hierarchy related functionality of a Guiro View.
  ]]
module("lost.guiro", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.HasSubviews" {}

function HasSubviews:constructor()
  self.subviews = {}
end

--[[
    Adds subview at the end of self.subviews and calls subview:onAttach(self)
  ]]
function HasSubviews:addSubview(subview)
  if subview:isDerivedFrom("lost.guiro.View") then
    table.insert(self.subviews, subview)
  	subview:onAttach(self)
  else
    local typeName = type(subview)
    if type(subview.className) == "function" then
      typeName = subview:className()
    end
    error("HasSubviews:addSubview() expected lost.guiro.View, got ".. typeName, 2)
  end
end

--[[
    Removes subview from self.subviews and calls subview:onDetach(self)
  ]]
function HasSubviews:removeSubview(subview)
  if subview:isDerivedFrom("lost.guiro.View") then
    for k,view in next,self.subviews do
      if rawequal(view, subview) then
        self.subviews[k] = nil
        view:onDetach(self)
      end
    end
  else
    local typeName = type(subview)
    if type(subview.className) == "function" then
      typeName = subview:className()
    end
    error("HasSubviews:removeSubview() expected lost.guiro.View, got ".. typeName, 2)
  end
end

function HasSubviews:removeAllSubviews()
  local idx = #self.subviews
  while idx > 0 do
    local view = table.remove(self.subviews, idx)
    view:onDetach(self)
    idx = idx-1
  end
end

--[[
    returns subview with given subviewId
    nil if subviewId is invalid
  ]]
function HasSubviews:__call(subviewId)
  local result = nil
  for k,view in next,self.subviews do
    if (view.id == subviewId) then
      result = view
      break
    end
  end
  return result
end

--breadth-first search for subview with given id
function HasSubviews:recursiveFindById(viewId)
  local result = self(viewId)
  if result == nil then
    for k,view in next,self.subviews do
      result = view:recursiveFindById(viewId)
      if result ~= nil then
        break
      end
    end
  end
  return result
end
