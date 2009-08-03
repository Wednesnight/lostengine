-- lost.declarative.Guiro
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Guiro" {}

function Guiro:create(loader)
  self.loader = loader
end

