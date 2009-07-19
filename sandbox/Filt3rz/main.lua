local loadShader = lost.gl.loadShader
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Rect = lost.math.Rect
local Color = lost.common.Color
local Camera2D = lost.camera.Camera2D
local Camera3D = lost.camera.Camera3D
local WindowParams = lost.application.WindowParams

function init()
  log.debug("init")

  fboSize = Vec2(256, 256)
  numPanels = 5
  numRows = 2
  screenSize = Vec2(fboSize.width * numPanels, fboSize.height * numRows)
  windowParams = WindowParams("Filt3rz", Rect(50, 200, screenSize.width, screenSize.height))
end

function startup(tasklet)
  log.debug("startup")
  framebuffer = lost.gl.FrameBuffer.createFrameBuffer(tasklet.window.context, fboSize, gl.GL_RGBA, 24)

  cam2D = Camera2D(Rect(0, 0, screenSize.width, screenSize.height))

  cam3D = Camera3D(Rect(0, 0, fboSize.width, fboSize.height))
  cam3D:fovY(45.0)
  cam3D:depth(Vec2(1.0, 1000.0))
  cam3D:position(Vec3(1,2,2))
  cam3D:target(Vec3(0,0,0))
  cam3D:stickToTarget(true)  

  blurShader = loadShader(tasklet.loader, "blur")
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
  sepiaShader:disable()

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
--  mesh.material = lost.mesh.Material.create()
  mesh.material.shader = lightShader

  return true;
end
