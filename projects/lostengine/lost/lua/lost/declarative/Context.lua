module("lost.declarative", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.declarative.Context" {}

function Context:create(loader)
  log.debug("dclarative.Context:create")
  self.loader = loader
end


