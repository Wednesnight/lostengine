module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: View
  ]]

require("lost.io.Loader")

View = { indices = {} }

function Loader:applyDefinitionToView(view, definition)
  local hasId = false

  -- apply listeners
  if definition.listeners then
    for name,value in next,definition.listeners do
      view:addEventListener(name, value)
    end
    definition.listeners = nil
  end

  -- apply properties
  if (view and definition) then
    for name,value in next,definition do
      if type(name) == "string" then
        hasId = hasId or (name == "id")
        view[name] = value
      end
    end
  end
  if not hasId then
    local name = view.bases[table.maxn(view.bases)]
    if (not View.indices[name]) then
      View.indices[name] = 1
    else
      View.indices[name] = View.indices[name]+1
    end

    view.id = name .. View.indices[name]
  end

  -- apply children
  if (view and definition) then
    for name,object in next,definition do
      if (type(object) == "userdata" and object.isView) then
        local info = class_info(object)
        view:appendChild(object)
      end
    end
  end

  return view
end
