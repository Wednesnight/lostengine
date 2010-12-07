module("lost.guiro.layout", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.layout.Layout" {}

function Layout:constructor()
  log.debug("layout constructor")
end

function Layout:apply(target, items)
  log.debug("apply")
end