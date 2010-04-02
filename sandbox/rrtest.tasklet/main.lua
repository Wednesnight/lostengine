require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.common.Color"
using "lost.bitmap.Bitmap"

using "lost.math.MatrixTranslation"

screensize = Vec2(512,512)
windowParams = WindowParams("rrtest", Rect(50,50,screensize.x, screensize.y))

running = true
rootNode = nil
dcl = nil

radius = 32
quadsize = Vec2(radius*2, radius*2)

-- creates a square texture with a white circle on transparent background
-- n is log2(width)
-- circle is centered and sized so it completely fills the bitmap used to create the texture
function pow2discTexture(n)
  local size = math.pow(2,n)
  local bmp = Bitmap.create(size, size, lost.bitmap.COMPONENTS_RGBA)
  local cx = size/2 - 0.5
  local cy = cx
  local cr = cy
  bmp:disc(cx,cy,cr)
  local defaultParams = lost.gl.Texture.Params()
  local tex = lost.gl.Texture.create(bmp, defaultParams)  
  return tex
end

function pow2ringTexture(n, thickness)
  local size = math.pow(2,n)
  local bmp = Bitmap.create(size, size, lost.bitmap.COMPONENTS_RGBA)
  local cx = size/2 - 0.5
  local cy = cx
  local cr = cy
  bmp:ring(cx,cy,cr, thickness)
  local defaultParams = lost.gl.Texture.Params()
  local tex = lost.gl.Texture.create(bmp, defaultParams)  
  return tex
end

function startup(tasklet)
  tasklet.name = "rrtest"
  tasklet.waitForEvents = true
  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  dcl = lost.declarative.Context(tasklet.loader)
  
  textureManager = lost.mesh.TextureManager.create()
  textureManager.maxRadius = 256 -- change this to test effect on quads with larger diameter
    
  ------------------
  -- ARC
  arcSize = 64
  spacing = 10
  arcBitmap = lost.bitmap.Bitmap.create(arcSize, arcSize, lost.bitmap.COMPONENTS_RGBA)
  arcBitmap:arc(arcSize, 4)

  arcFilledBitmap = lost.bitmap.Bitmap.create(arcSize, arcSize, lost.bitmap.COMPONENTS_RGBA)
  arcFilledBitmap:arcFilled(arcSize)
  
  texparams = lost.gl.Texture.Params()
  arcTex = lost.gl.Texture.create(arcBitmap, texparams)
  arcFilledTex = lost.gl.Texture.create(arcFilledBitmap, texparams)

  arcMesh = dcl.mesh:Quad
  {
    texture = arcTex, 
    material = 
    {
      blend = true
    },
    transform = MatrixTranslation(lost.math.Vec3(spacing,spacing,0))
  }

  arcFilledMesh = dcl.mesh:Quad
  {
    texture = arcFilledTex, 
    material = 
    {
      blend = true
    },
    transform = MatrixTranslation(lost.math.Vec3(2*spacing+arcSize,spacing,0))
  }

  debugQuad1 = dcl.mesh:Quad
  {
    rect = Rect(0,0,arcSize, arcSize),
    material = 
    {
      blend = true,
      color = Color(1,0,0)
    },
    transform = MatrixTranslation(lost.math.Vec3(spacing,spacing,0))
  }

  debugQuad2 = dcl.mesh:Quad
  {
    rect = Rect(0,0,arcSize, arcSize),
    material = 
    {
      blend = true,
      color = Color(0,1,0)
    },
    transform = MatrixTranslation(lost.math.Vec3(2*spacing+arcSize,spacing,0))
  }

  texmesh = dcl.mesh:Quad
  {
    texture = textureManager:arcFilledTexture(radius), -- pow2ringTexture(8, 1.5),
    material = 
    {
      blend = true
    },
    transform = MatrixTranslation(lost.math.Vec3(spacing,arcSize+2*spacing,0))
  }

  discmesh = lost.mesh.Disc.create(textureManager, true, arcSize, 0)
  discmesh.material.blend = true
  discmesh.material.shader = lost.common.Shaders.textureShader()
--  discmesh.material.color = Color(.8,.8,.8)
  discmesh.transform = MatrixTranslation(Vec3(spacing+arcSize, 2*spacing+arcSize, 0))
  
  rootNode = dcl.rg:Node
  {
    dcl.rg:ClearColor
    {
      color = Color(0,0,0,0)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT
    },
    dcl.rg:DepthTest{false},
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,screensize.x, screensize.y)
    },
    dcl.rg:Draw
    {
      active = false,
      mesh = dcl.mesh:Quad
      {
        rect = Rect(0,0,radius, radius),
        material =
        {
          color = Color(1,0,0)
        }
      }
    },
    dcl.rg:Draw
    {
      mesh = texmesh
    },
    dcl.rg:Draw
    {
      mesh = debugQuad1
    },
    dcl.rg:Draw
    {
      mesh = arcMesh
    },
    dcl.rg:Draw
    {
      mesh = debugQuad2
    },
    dcl.rg:Draw
    {
      mesh = arcFilledMesh
    },
    dcl.rg:Draw
    {
      mesh = discmesh
    },
  }

  tasklet.renderNode:add(rootNode)
  return true
end

function update(tasklet)
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  end
end
