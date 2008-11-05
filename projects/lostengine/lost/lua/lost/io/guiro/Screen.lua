module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Screen
  ]]

require("lost.io.Loader")
require("lost.guiro.Screen")

function Loader:loadScreen(filename)
  log.debug("Loader:load()")
  local result = {}
  local guiro = self
  guiro.Screen = function(self, definition) result = self:applyDefinitionToView(lost.guiro.Screen(), definition) end
  self:executeScript(filename, {guiro = guiro, lost = lost})
  return result
end

function Loader:applyDefinitionToView(view, definition)
  log.debug("Loader:applyDefinitionToView()")

  -- apply properties
  if (view and definition) then
    for name,value in next,definition do
      if type(name) == "string" then
        log.debug("setting property: ".. name)
        view[name] = value
      end
    end
  end

  -- apply children
  if (view and definition) then
    for name,object in next,definition do
      if (type(object) == "userdata" and object.isView) then
        local info = class_info(object)
        log.debug("setting child: ".. info.name)
        view:appendChild(object)
      end
    end
  end

  return view
end
