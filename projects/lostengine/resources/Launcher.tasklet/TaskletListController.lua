module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "TaskletListController" {}

function TaskletListController:constructor()
  self.view = require("ui_tasklets")
end
