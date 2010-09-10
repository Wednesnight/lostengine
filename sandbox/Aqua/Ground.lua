module("aqua", package.seeall)

require("lost.common.Class")

lost.common.Class "Ground" {}

function Ground:constructor()
  self.name = "Ground"
end

