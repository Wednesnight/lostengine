-- lost.declarative.Contex
module("lost.declarative", package.seeall)

require("lost.common.Class")
require("lost.declarative.Rg")
require("lost.declarative.Font")
require("lost.declarative.Mesh")

lost.common.Class "lost.declarative.Context" {}

function Context:create(loader)
  self.loader = loader
  self.rg = lost.declarative.Rg(self.loader)
  self.font = lost.declarative.Font(self.loader)
  self.mesh = lost.declarative.Mesh(self.loader)
end


