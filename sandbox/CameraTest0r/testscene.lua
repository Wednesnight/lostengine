local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local cam3dPosition = Vec3(1,2,2)

local light = lostgl:Texture
{
  filename = "texture1.JPG"
}

local thingTex = lostgl:Texture
{
  filename = "thing.png"
}

local sceneColor = Color(0.3,0.3,0.3)

gameState.light =
{
  mesh = lostmesh:Quad2D
  {
    texture = light,
    flip = true,
    size = Vec2(128, 128),
    material =
    {
      color = Color(0.5,0.5,0.5,0.75)
    }
  },
  rect = Rect(250, 180, 128, 128)
}

gameState.things =
{
  thing1 =
  {
    mesh = lostmesh:Quad2D
    {
      texture = thingTex,
      flip = true,
      size = Vec2(50,50),
      material =
      {
        color = Color(0.5,0.5,0.5)*sceneColor
      }
    },
    rect = Rect(0, 384, 50, 50)
  },
  thing2 =
  {
    mesh = lostmesh:Quad2D
    {
      texture = thingTex,
      flip = true,
      size = Vec2(50,50),
      material =
      {
        color = Color(0.5,0.5,0.5)*sceneColor
      }
    },
    rect = Rect(590, 288, 50, 50)
  },
  thing3 =
  {
    mesh = lostmesh:Quad2D
    {
      texture = thingTex,
      flip = true,
      size = Vec2(50,50),
      material =
      {
        color = Color(0.5,0.5,0.5)*sceneColor
      }
    },
    rect = Rect(0, 192, 50, 50)
  },
  thing4 =
  {
    mesh = lostmesh:Quad2D
    {
      texture = thingTex,
      flip = true,
      size = Vec2(50,50),
      material =
      {
        color = Color(0.5,0.5,0.5)*sceneColor
      }
    },
    rect = Rect(590, 96, 50, 50)
  }
}

lostrg:Scene
{
  name = "rootNode",
  lostrg:ClearColor{color = Color(0,0,0,1)},
  lostrg:Clear{mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT},
  lostrg:Node
  {
    name="2D Background",
    lostrg:Camera2D
    {
      name = "2D Cam",
      viewport = Rect(0,0,640,480) -- make this dependant on window size from config file
    },
    lostrg:DepthTest
    {
      false
    },
    lostrg:Draw
    {
      name = "line1",
      active = true,
      mesh = lostmesh:Line2D
      {
        start = Vec2(320, 96),
        finish = Vec2(640, 96),
        material =
        {
          color = Color(1,1,1)*sceneColor
        }
      }
    },
    lostrg:Draw
    {
      name = "line2",
      active = true,
      mesh = lostmesh:Line2D
      {
        start = Vec2(0, 192),
        finish = Vec2(320, 192),
        material =
        {
          color = Color(1,1,1)*sceneColor
        }
      }
    },
    lostrg:Draw
    {
      name = "line3",
      active = true,
      mesh = lostmesh:Line2D
      {
        start = Vec2(320, 288),
        finish = Vec2(640, 288),
        material =
        {
          color = Color(1,1,1)*sceneColor
        }
      }
    },
    lostrg:Draw
    {
      name = "line4",
      active = true,
      mesh = lostmesh:Line2D
      {
        start = Vec2(0, 384),
        finish = Vec2(320, 384),
        material =
        {
          color = Color(1,1,1)*sceneColor
        }
      }
    },
    lostrg:Draw
    {
      name = "thing1",
      active = true,
      mesh = gameState.things.thing1.mesh
    },
    lostrg:Draw
    {
      name = "thing2",
      active = true,
      mesh = gameState.things.thing2.mesh
    },
    lostrg:Draw
    {
      name = "thing3",
      active = true,
      mesh = gameState.things.thing3.mesh
    },
    lostrg:Draw
    {
      name = "thing4",
      active = true,
      mesh = gameState.things.thing4.mesh
    },
    lostrg:Blend{enable = true},
    lostrg:Draw
    {
      name = "flashlight",
      active = true,
      mesh = gameState.light.mesh
    },
    lostrg:Blend{enable = false}
  }
}
