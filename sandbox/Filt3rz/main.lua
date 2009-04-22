local loadShader = lost.gl.loadShader
local Vec2 = lost.math.Vec2
local Color = lost.common.Color

-- fboSize must be set from C++
-- cubeCam must be set from C++ as a Camera3DPtr

function init(loader)
  log.debug("init")
  
  blurShader = loadShader(loader, "blur")
  blurShader:enable()
  blurShader:setFloat("width", fboSize.width)
  blurShader:setFloat("height", fboSize.height)
  blurShader:setInt("colorMap", 0)
  blurShader:disable()

  edgeShader = loadShader(loader, "edge")
  edgeShader:enable()
  edgeShader:setFloat("width", fboSize.width)
  edgeShader:setFloat("height", fboSize.height)
  edgeShader:setInt("colorMap", 0)
  edgeShader:disable()

  embossShader = loadShader(loader, "emboss")
  embossShader:enable()
  embossShader:setFloat("width", fboSize.width)
  embossShader:setFloat("height", fboSize.height)
  embossShader:setInt("colorMap", 0)
  embossShader:disable()

  sharpenShader = loadShader(loader, "sharpen")
  sharpenShader:enable()
  sharpenShader:setFloat("width", fboSize.width)
  sharpenShader:setFloat("height", fboSize.height)
  sharpenShader:setInt("colorMap", 0)
  sharpenShader:disable()

  radialShader = loadShader(loader, "radial")
  radialShader:enable()
  radialShader:setInt("colorMap", 0)
  radialShader:disable()

  ssaoShader = loadShader(loader, "ssao")
  ssaoShader:enable()
  ssaoShader:setInt("texture0", 1)
  ssaoShader:setInt("texture1", 0)
  ssaoShader:set("camerarange", cubeCam:depth())
  ssaoShader:set("screensize", fboSize)
  ssaoShader:disable()

  sepiaShader = loadShader(loader, "sepia")
  sepiaShader:enable()
  sepiaShader:setInt("tex", 0)
  sepiaShader:disable()

  heatsigShader = loadShader(loader, "heatsig")
  heatsigShader:enable()
  heatsigShader:setInt("tex", 0)
  heatsigShader:disable()

  lightShader = loadShader(loader, "light")
  lightShader:enable()
  lightShader:validate()
  if not lightShader:validated() then
    log.debug("Problem found during validation: \n"..lightShader:log())
  else
    log.debug("Program validated OK")
  end
  lightShader:set("LightPosition", cubeCam:position())
  lightShader:set("LightColor", Color(1, 1, 1))
  lightShader:set("EyePosition", cubeCam:position())
  lightShader:set("Specular",  Color(.75, .75, .5))
  lightShader:set("Ambient",  Color(.1, .1, .1))
  lightShader:setFloat("Kd", 0.8)
  lightShader:set("Scale", Vec2(0.7, 3.7))
  lightShader:set("Threshold", Vec2(.3, .2))
  lightShader:set("SurfaceColor", Color(1,1,1))
  lightShader:disable()
  
end