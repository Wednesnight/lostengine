module("aqua", package.seeall)

require("lost.common.Class")

lost.common.Class "Entity" {}

function Entity:constructor()
  self.renderNode = lost.rg.Node.create()
  self.name = "<unset>"
  self.pos = Vec2(0,0)
end

function Entity:init(world)
  return true
end

function Entity:update(dt, world)
end

