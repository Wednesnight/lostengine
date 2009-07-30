-- lost.declarative.Contex
module("lost.declarative", package.seeall)

require("lost.common.Class")
require("lost.declarative.Rg")
require("lost.declarative.Font")
require("lost.declarative.Mesh")
require("lost.declarative.Gl")
require("lost.declarative.Resource")
require("lost.declarative.Guiro2")

lost.common.Class "lost.declarative.Context"
{
  log.debug("--- table shit")
}

function Context:create(loader)
  log.debug("--- create shit")
  self.loader = loader
  self.rg = lost.declarative.Rg(self.loader)
  self.font = lost.declarative.Font(self.loader)
  self.mesh = lost.declarative.Mesh(self.loader)
  self.gl = lost.declarative.Gl(self.loader)
  self.resource = lost.declarative.Resource(self.loader)
  self.guiro2 = lost.declarative.Guiro2(self.loader)
end


