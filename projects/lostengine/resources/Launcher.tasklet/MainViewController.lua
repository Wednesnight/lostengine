module("lost.common", package.seeall)

require("lost.common.Class")
require("SettingsController")
require("TaskletListController")

lost.common.Class "MainViewController" {}

function MainViewController:constructor()
  self.view = require("ui")
  self._listView = self.view:recursiveFindById("listview")
  self.view:addEventListener("drop",function(event) self:dropEventHandler(event) end)
  self.settingsController = SettingsController()
  self.taskletController = TaskletListController()
  self._listView:addItem("Tasklets",self.taskletController.view)
  self._listView:addItem("Settings",self.settingsController.view)
  self._listView:select(1)
end

function MainViewController:dropEventHandler(event)
  local originalEvent = event.lostAppDragNDropEvent
  for i=0,math.max(0,originalEvent:numPaths()-1) do
    local p = originalEvent:getPath(i)
    tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(p))
  end
end
