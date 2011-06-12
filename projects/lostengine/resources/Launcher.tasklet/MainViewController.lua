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
  local originalEvent = event.lostAppDragNDropEvent
  for i=0,math.max(0,originalEvent:numPaths()-1) do
    local p = originalEvent:getPath(i)
    tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(p))
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
    local bp = boost.filesystem.path(entry.path)
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
  self.taskletController:tasklets(self._prefs.tasklets)
end

function MainViewController:shutdown()
  self._prefsManager:save(self._prefs)
end

