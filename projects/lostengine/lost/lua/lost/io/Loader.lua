module("lost.io", package.seeall)

require("lost.common.Class")

--[[
     Loader
  ]]

lost.common.Class "lost.io.Loader" {}

function Loader:create(loader)
  log.debug("Loader:create()")
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

function Loader:File(filename)
  return self.loader:load(filename)
end
