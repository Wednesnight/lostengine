module("aqua", package.seeall)

require("lost.common.Class")

lost.common.Class "Entity" {}

function Entity:constructor()
  self.renderNode = lost.rg.Node.create()
  self.name = "<unset>"
end

function Entity:update(dt, world)
  
end

