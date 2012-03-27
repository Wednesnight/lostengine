module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "SettingsController" {}

function SettingsController:constructor()
  self.view = require("ui_settings")
end
