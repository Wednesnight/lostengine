require("lost.common.Class")

lost.common.Class "Entity" {}

-- animatable entity, can hold other entities
-- has an update method
-- has an rg node
function Entity:create()
  self.children = {}
  self.rootNode = lost.rg.Node.create()
end

function Entity:add(entity)
  self.rootNode:add(entity.rootNode)
  local pos = table.maxn(self.children)+1
  table.insert(self.children, pos, entity)
end

function Entity:updateChildren(tasklet, dt, at)
  for k,v in pairs(self.children) do
    v:update(tasklet, dt, at)
  end
end

function Entity:update(tasklet, dt, at)
  self:updateChildren(tasklet, dt, at)
end