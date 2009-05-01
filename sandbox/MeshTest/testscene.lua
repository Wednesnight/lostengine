local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color

rg:Scene
{
  name = "rootNode",
  rg:Node
  {
    name="2D Background",
    rg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,640,480) -- make this dependant on window size from config file
    },
    rg:DepthTest
    {
      false
    },
    rg:Draw
    {
      name = "big zim",
      active = true,
      mesh = mesh:Quad2D
      {
        filename = "zim.png",
        flip = true,
        size = Vec2(640,480)
      }
    },
    rg:Draw
    {
      name = "little red test quad",
      mesh = mesh:Quad2D
      {
        rect = Rect(0,0,3,3),
        material =
        {
          color = Color(1,0,0)
        }
      }
    }    
  },
  rg:Node
  {
    name="3D Foreground",
    rg:Camera3D
    {
      name = "3D Cam",
      viewport = Rect(0,0,640,480), -- make this dependant on window size from config file
      fovY = 45,
      depth = Vec2(1,1000),
      position = Vec3(1,2,2),
      target = Vec3(0,0,0),
      stickToTarget = true
    }
  }
}