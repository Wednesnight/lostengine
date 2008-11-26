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

function Loader:executeScript(filename, environment)
  log.debug("Loader:executeScript()")
  local file = self.loader:load(filename)
  local script = file:str()
  local func,error = loadstring(script, filename)
  if (func) then
    setfenv(func, environment)
    local result,error = pcall(func)
    if (not result) then
      log.error("error while executing '".. filename .."': ".. error)
    end
  else
    log.error("error while loading '".. filename .."': ".. error)
  end
end

--[[
    overwrite values in table "target" with values from table "source"
  ]]
function Loader:apply(target, source)
  if (target and source) then
    for key,value in next,source do
      target[key] = value
    end
  end
  return target
end
