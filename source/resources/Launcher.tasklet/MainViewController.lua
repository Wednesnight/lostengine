module("lost.common", package.seeall)

require("lost.common.Class")
require("SettingsController")
require("TaskletListController")
require("lost.common.PrefsManager")

lost.common.Class "MainViewController" {}

local defaultPrefs = {tasklets={}}

function MainViewController:constructor()
  self.view = require("ui")
  self._listView = self.view:recursiveFindById("listview")
  self.view:addEventListener("drop",function(event) self:dropEventHandler(event) end)
  self.settingsController = SettingsController()
  self.taskletController = TaskletListController()
  self._listView:addItem("Tasklets",self.taskletController.view)
  self._listView:addItem("Settings",self.settingsController.view)
  self._listView:select(1)
  self._prefsManager = lost.common.PrefsManager{prefix="LostEngine/Launcher","prefs.lua"}
  self._prefs = defaultPrefs
end

function MainViewController:dropEventHandler(event)
  for i=0,math.max(0,event:numPaths()-1) do
    local p = event:getPath(i)
    tasklet:spawnTasklet(p)
    self:addTaskletPath(p)
  end
end

function MainViewController:addTaskletPath(p)
  -- check if it exists already
  local entry = nil
  for k,v in pairs(self._prefs.tasklets) do
    if v.path == p then
      entry = v
      break
    end
  end
  if not entry then
    entry = {path=p}
    local bp = lost.fs.Path(entry.path)
    bp = bp / "config.lua"
    local file = io.open(bp:native(),"r")
    local data = file:read("*all")
    file:close()
    local f,err = loadstring(data)
    if f and not err then
      local taskletConfig = f()
      entry.name = taskletConfig.tasklet.name or "<unnamed>"
    else
      log.warn("no config file or broken config for tasklet: "..tostring(entry.path))
    end
    table.insert(self._prefs.tasklets,entry)
    self.taskletController:tasklets(self._prefs.tasklets)
  end
end

function MainViewController:startup()
  self._prefs = self._prefsManager:load() or defaultPrefs

  -- make sure there's an autorun flag in the tasklet entry
  for k,v in pairs(self._prefs.tasklets) do
    if v.autorun == nil then
      v.autorun = false
    end
  end

  -- start autorun tasklets
  for k,v in pairs(self._prefs.tasklets) do
    if v.autorun == true then
      tasklet:spawnTasklet(v.path)   
    end
  end

  self.taskletController:tasklets(self._prefs.tasklets)
end

function MainViewController:shutdown()
  self._prefsManager:save(self._prefs)
end

