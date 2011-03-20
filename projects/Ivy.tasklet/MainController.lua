require("lost.common.Class")

lost.common.Class "MainController" {}

function MainController:constructor()
  log.debug("maincontroller constructor")
end

function MainController:fileDropped(path)
  log.debug(path)
end

