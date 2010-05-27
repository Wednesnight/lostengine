require("lost.common.Class")

lost.common.Class "BlockAnimation" {}

function BlockAnimation:constructor(blockCoords)
  self.coords = blockCoords
end

function BlockAnimation:init(manager)
  self.manager = manager
end
