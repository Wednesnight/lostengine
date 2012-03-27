module("lost.common", package.seeall)

require("lost.common.Class")
require("lost.common.Serializer")

lost.common.Class "lost.common.PrefsManager" {}

-- Shortcut for maintaining a prefs file in userdata directory.
-- construct with table parameter:
-- local prefs = Prefs{...}
-- prefix : directory prefix, will be appended to userDataPath
-- filename : will be appended to userdatapath + prefix
function PrefsManager:constructor(t)
  self._prefix = t.prefix or "LostEngine"
  self._filename = t.filename or "prefs.lua"
  self._prefsFilePath = lost.platform.getUserDataPath() / self._prefix / self._filename
end

function PrefsManager:load()
  local result = nil
  if lost.fs.exists(self._prefsFilePath) then
    local file = io.open(self._prefsFilePath:native())
    local data = file:read("*all")
    file:close()
    local f,err = loadstring(data)
    if not f and err then
      log.error("!! broken prefs file : "..self._prefsFilePath:native())
    else
      result = f()
    end
  else
--    log.debug("no prefs found, returning empty table")
  end
  return result
end

function PrefsManager:save(t)
  local s = lost.common.Serializer()
  local data = s:serialize(t)
  
  if not lost.fs.exists(self._prefsFilePath) then
    log.debug("no prefs path found, creating")
    local dirpath = lost.platform.getUserDataPath() / self._prefix
    lost.fs.create_directories(dirpath)
  end
  
  local prefsFile = io.open(self._prefsFilePath:native(),"w+")
  prefsFile:write(data)
  prefsFile:close()
end