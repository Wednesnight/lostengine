-- lost.declarative.Guiro2
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Guiro2" {}

function Guiro2:create(loader)
  self.loader = loader
end

