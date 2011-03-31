module("meta",package.seeall)

require("lost.common.Class")
require("Plane")

local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect

lost.common.Class "meta.Cube" {}
-- Params
-- size : size of the cube in one direction
-- numPlanes : number of planes in one direction
-- color : will be applied to all planes

function Cube:constructor(args)
  self.size = args.size
  self.numPlanes = args.numPlanes
  self.color = args.color
  self.planes = {}
  self.renderNode = lost.rg.Node.create()
  self:createXY(args)
end

function Cube:createXY(args)
  local spaceStep=self.size/(self.numPlanes-1)
  local metaStep=1/(self.numPlanes-1)
  local curSpace = 0
  local curMeta = 0
  for i=1,self.numPlanes do
    local plane = meta.Plane{mode="xy",color=self.color,rect=Rect(0,0,self.size,self.size),meshOffset=curSpace,metaOffset=curMeta}
    curSpace = curSpace + spaceStep
    curMeta = curMeta + metaStep
    self.renderNode:add(lost.rg.Draw.create(plane.mesh))
    table.insert(self.planes,plane)
  end
end