module("lost.io", package.seeall)

--[[
     Loader
  ]]

class "lost.io.Loader"
Loader = _G["lost.io.Loader"]

function Loader:__init(loader)
  log.debug("Loader:__init()")
  self.loader = loader
end

function Loader:load(filename)
  log.debug("Loader:load()")
  local file = self.loader:load(filename)
  local script = file:str()
  local func,error = loadstring(script, filename)
  if (func) then
    setfenv(func, {guiro = self, lost = lost})
    local result,error = pcall(func)
    if (not result) then
      log.error("error while executing '".. filename .."': ".. error)
    end
  else
    log.error("error while loading '".. filename .."': ".. error)
  end
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
