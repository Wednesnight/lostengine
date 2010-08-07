require("lost.common.Shaders")

using "lost.gl.loadShader"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.camera.Camera2D"
using "lost.camera.Camera3D"
using "lost.application.WindowParams"
using "lost.math.MatrixTranslation"

fboSize = Vec2(256, 256)
numPanels = 5
numRows = 2
screenSize = Vec2(fboSize.width * numPanels, fboSize.height * numRows)

hasWindow = true
windowParams = WindowParams("Filt3rz", Rect(50, 200, screenSize.width, screenSize.height))

-- blur edge emboss sharpen
-- radial ssao sepia heatsig
function addTextNode(rootNode, panelText, colIndex, rowIndex, col)
    local txt = ttf:render(panelText, fontSize)
    local xOffset = (colIndex * fboSize.width) + (fboSize.width - txt.size.width) / 2
    local yOffset = (rowIndex+1) * fboSize.height - yinset
    txt.material.color = col;
    txt.material.shader = lost.common.Shaders.textureShader()
    txt.transform = MatrixTranslation(Vec3(xOffset, yOffset, 0))
    rootNode:add(lost.rg.Draw.create(txt))
end

function addPanelNode(rootNode, tex, colIndex, rowIndex, theShader)
  local quad = lost.mesh.Quad.create(tex, false)
  quad.transform = MatrixTranslation(Vec3(colIndex*fboSize.width,rowIndex*fboSize.height,0));  
  if theShader then
    quad.material.shader = theShader
  end
  rootNode:add(lost.rg.Draw.create(quad))
end

function startup()
  tasklet.name = "Filt3rz"
  log.debug("startup")

  running = true
  filterzTasklet = tasklet
  passedSec = lost.platform.currentTimeSeconds()
  angle = 0
  animated = true

  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)

  ftlib = lost.font.freetype.Library.create()
  fontSize = 30
  yinset = fontSize+3  

  fontData = tasklet.loader:load("miserable.ttf")
  ttf = lost.font.TrueTypeFont.create(ftlib, fontData)

  cam2D = Camera2D.create(Rect(0, 0, screenSize.width, screenSize.height))

  cam3D = Camera3D.create(Rect(0, 0, fboSize.width, fboSize.height))
  cam3D:fovY(45.0)
  cam3D:depth(Vec2(1.0, 1000.0))
  cam3D:position(Vec3(1,2,2))
  cam3D:target(Vec3(0,0,0))
  cam3D:stickToTarget(true)  

--[[  blurShader = loadShader(tasklet.loader, "blur")
  blurShader:enable()
  blurShader:setFloat("width", fboSize.width)
  blurShader:setFloat("height", fboSize.height)
  blurShader:setInt("colorMap", 0)
  blurShader:disable()

  edgeShader = loadShader(tasklet.loader, "edge")
  edgeShader:enable()
  edgeShader:setFloat("width", fboSize.width)
  edgeShader:setFloat("height", fboSize.height)
  edgeShader:setInt("colorMap", 0)
  edgeShader:disable()

  embossShader = loadShader(tasklet.loader, "emboss")
  embossShader:enable()
  embossShader:setFloat("width", fboSize.width)
  embossShader:setFloat("height", fboSize.height)
  embossShader:setInt("colorMap", 0)
  embossShader:disable()

  sharpenShader = loadShader(tasklet.loader, "sharpen")
  sharpenShader:enable()
  sharpenShader:setFloat("width", fboSize.width)
  sharpenShader:setFloat("height", fboSize.height)
  sharpenShader:setInt("colorMap", 0)
  sharpenShader:disable()

  radialShader = loadShader(tasklet.loader, "radial")
  radialShader:enable()
  radialShader:setInt("colorMap", 0)
  radialShader:disable()

  ssaoShader = loadShader(tasklet.loader, "ssao")
  ssaoShader:enable()
  ssaoShader:setInt("texture0", 1)
  ssaoShader:setInt("texture1", 0)
  ssaoShader:set("camerarange", cam3D:depth())
  ssaoShader:set("screensize", fboSize)
  ssaoShader:disable()

  sepiaShader = loadShader(tasklet.loader, "sepia")
  sepiaShader:enable()
  sepiaShader:setInt("tex", 0)
  sepiaShader:disable()]]

  heatsigShader = loadShader(tasklet.loader, "heatsig")
  heatsigShader:enable()
  heatsigShader:setInt("tex", 0)
  heatsigShader:disable()

  lightShader = loadShader(tasklet.loader, "light")
  lightShader:enable()
  lightShader:validate()
  if not lightShader:validated() then
    log.debug("Problem found during validation: \n"..lightShader:log())
  else
    log.debug("Program validated OK")
  end
  lightShader:set("LightPosition", cam3D:position())
  lightShader:set("LightColor", Color(1, 1, 1))
  lightShader:set("EyePosition", cam3D:position())
  lightShader:set("Specular",  Color(.75, .75, .5))
  lightShader:set("Ambient",  Color(.1, .1, .1))
  lightShader:setFloat("Kd", 0.8)
  lightShader:set("Scale", Vec2(0.7, 3.7))
  lightShader:set("Threshold", Vec2(.3, .2))
  lightShader:set("SurfaceColor", Color(1,1,1))
  lightShader:disable()

  mesh = lost.mesh.Loader.obj(tasklet.loader:load("magnolia_tri.obj"))
  mesh.material.shader = lightShader

  -- create the node that actually does the drawing of the 3D object into the framebuffer
  scene = lost.rg.Node.create()
  
  local framebuffer = lost.gl.FrameBuffer.create(fboSize, gl.GL_RGBA8, gl.GL_DEPTH_COMPONENT24, -1)
  fbnode = lost.rg.Node.create()
  fbnode:add(lost.rg.FrameBuffer.create(framebuffer))
  fbnode:add(lost.rg.Camera.create(cam3D))
  fbnode:add(lost.rg.ClearColor.create(Color(1,1,1)))
  fbnode:add(lost.rg.DepthTest.create(true))
  fbnode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  fbnode:add(lost.rg.Draw.create(mesh))
  fbnode:add(lost.rg.DefaultFrameBuffer.create())
  
  -- build the panels that render the framebuffer color attachment 0 wirh the appropriate shader for the panel
  
  panelsNode = lost.rg.Node.create()
  panelsNode:add(lost.rg.Camera.create(cam2D))
  panelsNode:add(lost.rg.ClearColor.create(Color(.5,.5,.5)))
  panelsNode:add(lost.rg.Clear.create(gl.GL_COLOR_BUFFER_BIT+gl.GL_DEPTH_BUFFER_BIT))
  panelsNode:add(lost.rg.DepthTest.create(false))

  local fbtex = framebuffer:colorTexture(0)
  addPanelNode(panelsNode, fbtex, 0,1)
  addPanelNode(panelsNode, fbtex, 1,1, blurShader)
  addPanelNode(panelsNode, fbtex, 2,1, edgeShader)
  addPanelNode(panelsNode, fbtex, 3,1, embossShader)
  addPanelNode(panelsNode, fbtex, 4,1, sharpenShader)
  addPanelNode(panelsNode, fbtex, 0,0, radialShader)
  addPanelNode(panelsNode, fbtex, 1,0, ssaoShader)
  addPanelNode(panelsNode, fbtex, 2,0, sepiaShader)
  addPanelNode(panelsNode, fbtex, 3,0, heatsigShader)
  
  scene:add(fbnode)
  scene:add(panelsNode)
  
  -- build text display
  rootTextNode = lost.rg.Node.create()
  local blackColor = Color(0,0,0)
  local whiteColor = Color(1,1,1)
  addTextNode(rootTextNode, "Original", 0, 1, blackColor)
  addTextNode(rootTextNode, "Gaussian Blur", 1, 1, blackColor)
  addTextNode(rootTextNode, "Edge Detect", 2, 1, whiteColor)
  addTextNode(rootTextNode, "Emboss", 3, 1, whiteColor)
  addTextNode(rootTextNode, "Sharpen", 4, 1, blackColor)
  addTextNode(rootTextNode, "Radial Blur", 0, 0, blackColor)
  addTextNode(rootTextNode, "SSAO", 1, 0, blackColor)
  addTextNode(rootTextNode, "Sepia", 2, 0, blackColor)
  addTextNode(rootTextNode, "Heat Signature", 3, 0, blackColor)

  scene:add(rootTextNode)
  
  -- add to tasklet node
  tasklet.renderNode:add(scene)

  return true;
end

function update()
  local currentSec = lost.platform.currentTimeSeconds()
  local delta = currentSec - passedSec
  
  if animated then
    angle = math.fmod(delta*50+angle, 360)
    mesh.transform = lost.math.MatrixRotX(angle) * lost.math.MatrixRotY(angle)
  end

  passedSec = currentSec
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  elseif (event.key == lost.application.K_SPACE) then
    animated = not animated
  end
end
