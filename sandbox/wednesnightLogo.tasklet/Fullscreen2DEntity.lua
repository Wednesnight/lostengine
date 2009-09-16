require("Entity")

lost.common.Class "Fullscreen2DEntity" "Entity" {}

local Rect = lost.math.Rect
local Vec3 = lost.math.Vec3
local Vec2 = lost.math.Vec2
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local MatrixRotZ = lost.math.MatrixRotZ

function Fullscreen2DEntity:constructor(bgcol)
  Entity.constructor(self)
  self.backgroundColor = bgcol
  self.rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor{color = self.backgroundColor},
    dcl.rg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,windowRect.width,windowRect.height)
    },
    dcl.rg:DepthTest
    {
      false
    },
  }
end
