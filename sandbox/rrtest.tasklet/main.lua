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
  
  log.debug("disc: " .. tostring(lost.mesh.Disc))

  for k,v in pairs(lost.mesh) do
    log.debug(k .. " : " .. tostring(v))
  end

  textureManager = lost.mesh.TextureManager.create()

  textureManager.maxDiameter = 256 -- change this to test effect on quads with larger diameter
--  textureManager._radiusOffset = -.5
--  textureManager._centerOffset = -.5

  texmesh = dcl.mesh:Quad
  {
    texture = textureManager:discTexture(radius), -- pow2ringTexture(8, 1.5),
    size = Vec2(radius, radius), -- blow up quad to desired size, irrespective of actual texture size
    material = 
    {
      blend = true
    },
    transform = MatrixTranslation(lost.math.Vec3(0,0,0))
  }
  
  local discradius = 50
  discmesh = lost.mesh.Disc.create(textureManager, discradius)
  discmesh.material.blend = true
  discmesh.material.shader = lost.common.Shaders.textureShader()
  discmesh.transform = MatrixTranslation(Vec3(discradius+10, discradius+10, 0))
  
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
      mesh = texmesh -- discmesh --lost.mesh.Disc.create(textureManager, 50) -- texmesh
    }
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
  elseif (event.key == lost.application.K_A) then
    quadsize = Vec2(quadsize.x+1, quadsize.y+1)
    texmesh:updateSize(quadsize)
  elseif (event.key == lost.application.K_S) then
    quadsize = Vec2(quadsize.x-1, quadsize.y-1)
    texmesh:updateSize(quadsize)
  end
  
end
