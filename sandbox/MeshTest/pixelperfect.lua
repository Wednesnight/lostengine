local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation

local lines = lost.rg.Node.create()
for i=0,screenSize.x-1,3 do
  lines:add(lost.rg.Draw.create(lost.mesh.Line2D.create(Vec2(i,0), Vec2(i,screenSize.y-1))))
end
for i=0,screenSize.y-1,3 do
  lines:add(lost.rg.Draw.create(lost.mesh.Line2D.create(Vec2(0,i), Vec2(screenSize.x-1,i))))
end

local frame = lost.rg.Node.create()
local l=lost.mesh.Line2D.create(Vec2(0,0), Vec2(0,480))
l.material.color = Color(1,0,0)
frame:add(lost.rg.Draw.create(l))

lostrg:Scene
{
	name = "rootNode",
  lostrg:ClearColor{ color = Color(0,0,0,1) },
  lostrg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
	lostrg:Camera2D
  {
    name = "2D Cam",
    viewport = Rect(0,0,screenSize.x,screenSize.y)
  },
  lostrg:DepthTest
  {
    false
  },
  lines,
  frame
}