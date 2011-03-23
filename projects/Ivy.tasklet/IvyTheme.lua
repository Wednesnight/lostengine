require("lost.common.Class")
require("lost.guiro.Theme")

lost.common.Class "IvyTheme" "lost.guiro.Theme" {}

function IvyTheme:constructor()
  lost.guiro.Theme.constructor(self)
  self.name = "ivy"
  
  self:addStyle("lost.guiro.view.Button", "deleteButton", function(target, args) self:buttonRounded(target, args) end)  
end

function IvyTheme:deleteButton(target,args)
  log.debug("delete button style")
end
