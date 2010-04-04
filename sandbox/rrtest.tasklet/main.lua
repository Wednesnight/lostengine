require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.common.Color"
using "lost.common.ColorPoint"
using "lost.common.ColorGradient"
using "lost.bitmap.Bitmap"

using "lost.math.MatrixTranslation"

screensize = Vec2(800,600)
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
  arcSize = 16
  spacing = 10
  arcBitmap = lost.bitmap.Bitmap.create(arcSize, arcSize, lost.bitmap.COMPONENTS_RGBA)
  arcBitmap:ring(0,0,arcSize, 1)

  arcFilledBitmap = lost.bitmap.Bitmap.create(arcSize, arcSize, lost.bitmap.COMPONENTS_RGBA)
  arcFilledBitmap:disc(0,0,arcSize)
  
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

  discmesh = lost.mesh.Disc.create(textureManager, false, arcSize, 2)
  discmesh.material.blend = true
  discmesh.material.shader = lost.common.Shaders.textureShader()
--  discmesh.material.color = Color(1,1,1, .2)
  discmesh.transform = MatrixTranslation(Vec3(spacing+arcSize, 2*spacing+arcSize, 0))



  rr = lost.mesh.RoundedRect.create(textureManager, Vec2(64, 64), false, 8, 1);
  rr.material.blend = true
  rr.material.shader = lost.common.Shaders.textureShader()
  rr.transform = MatrixTranslation(Vec3(spacing, 200, 0))

  rr2 = lost.mesh.RoundedRect.create(textureManager, Vec2(64, 64), true, 8, 1);
  rr2.material.blend = true
  rr2.material.color = Color(0,.6, .9, .5);
  rr2.material.shader = lost.common.Shaders.textureShader()
  rr2.transform = MatrixTranslation(Vec3(spacing, 200, 0))

  rr3 = lost.mesh.RoundedRect.create(textureManager, Vec2(16, 16), true, 8, 2);
  rr3.material.blend = true
  rr3.material.color = Color(1,.6, .9, 1);
  rr3.material.shader = lost.common.Shaders.textureShader()
  rr3.transform = MatrixTranslation(Vec3(spacing+80, 200, 0))
  rr3:updateCorners(false, true, true, true)

  
  -- GRADIENT  
  g1 = ColorGradient.create()
  g1:add(ColorPoint(0,Color(1,0,0)))
  g1:add(ColorPoint(.5,Color(0,1,0)))
  g1:add(ColorPoint(1,Color(0,0,1)))

  g2 = ColorGradient.create()
  g2:add(ColorPoint(0,Color(0,0,0)))
  g2:add(ColorPoint(1,Color(1,1,1)))

  g3 = ColorGradient.create()
  g3:add(ColorPoint(0,Color(1,1,0)))
  g3:add(ColorPoint(.25,Color(1,.5,0)))
  g3:add(ColorPoint(.75,Color(.75,.8,.2)))
  g3:add(ColorPoint(1,Color(.8,.8,.8)))

  g4 = ColorGradient.create()
  g4:add(ColorPoint(0,Color(1,1,1)))
  g4:add(ColorPoint(1,Color(0,0,0)))

  g1coord = textureManager:addGradient(g1)
  g2coord = textureManager:addGradient(g2)
  g3coord = textureManager:addGradient(g3)
  g4coord = textureManager:addGradient(g4)

--  log.debug("!!!!!! gradcoord "..textureManager:addGradient(g1));
--  log.debug("!!!!!! gradcoord "..textureManager:addGradient(g2));
--  log.debug("!!!!!! gradcoord "..textureManager:addGradient(g3));
--  log.debug("!!!!!! gradcoord "..textureManager:addGradient(gradient));

  gradGeometryDelta = 0
  gradDisc = lost.mesh.Disc.create(textureManager, true, arcSize-gradGeometryDelta, 2)
  gradDisc.material.blend = true
  gradDisc.material.shader = lost.common.Shaders.gradientShader()
  gradDisc.transform = MatrixTranslation(Vec3(2*spacing+3*arcSize+gradGeometryDelta/2, 2*spacing+arcSize+gradGeometryDelta/2, 0))
  gradDisc.material.uniforms = lost.gl.UniformBlock.create()
  gradDisc.material.uniforms:setFloat("gradientCoord", g2coord)
  gradDisc.material:add(textureManager.gradientTexture)

  gradRing = lost.mesh.Disc.create(textureManager, false, arcSize, 3)
  gradRing.material.blend = true
  gradRing.material.shader = lost.common.Shaders.gradientShader()
  gradRing.transform = MatrixTranslation(Vec3(2*spacing+3*arcSize, 2*spacing+arcSize, 0))
  gradRing.material.uniforms = lost.gl.UniformBlock.create()
  gradRing.material.uniforms:setFloat("gradientCoord", g3coord)
  gradRing.material:add(textureManager.gradientTexture)

  rr4 = lost.mesh.RoundedRect.create(textureManager, Vec2(64, 64), true, 8, 2);
  rr4.material.blend = true
  rr4.material.shader = lost.common.Shaders.gradientShader()
  rr4.transform = MatrixTranslation(Vec3(2*spacing+80+20, 200, 0))
  rr4.material.uniforms = lost.gl.UniformBlock.create()
  rr4.material.uniforms:setFloat("gradientCoord", g3coord)
  rr4.material:add(textureManager.gradientTexture)
  rr4:updateCorners(false, false, false, true)

  gradmesh = dcl.mesh:Quad
  {
    texture = textureManager.gradientTexture,
    material = 
    {
      blend = true
    },
    transform = MatrixTranslation(lost.math.Vec3(300,spacing,0))
  }

  ub = lost.gl.UniformBlock.create()
  ub:set("color", Color(0,1,1))
  rr3.material.uniforms = ub
  
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
--    dcl.rg:Draw{mesh = texmesh},
    dcl.rg:Draw{mesh = debugQuad1},
    dcl.rg:Draw{mesh = arcMesh},
    dcl.rg:Draw{mesh = debugQuad2},
    dcl.rg:Draw{mesh = arcFilledMesh},
    dcl.rg:Draw{mesh = discmesh},
    dcl.rg:Draw{mesh = rr2},
    dcl.rg:Draw{mesh = rr},
    dcl.rg:Draw{mesh = rr3},
    dcl.rg:Draw{mesh = gradmesh},
    dcl.rg:Draw{mesh = gradDisc},
    dcl.rg:Draw{mesh = gradRing},
    dcl.rg:Draw{mesh = rr4},
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
