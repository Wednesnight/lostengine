require("lost.declarative.Context")

using "lost.math.Vec2"
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

radius = 67
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
  texmesh = dcl.mesh:Quad
  {
    texture = textureManager:discTexture(radius), -- pow2ringTexture(8, 1.5),
    material = 
    {
      blend = true
    },
    transform = MatrixTranslation(lost.math.Vec3(10,10,0))
  }
  
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
    dcl.rg:DepthTest{true},
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,screensize.x, screensize.y)
    },
    dcl.rg:Draw
    {
      mesh = texmesh
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
