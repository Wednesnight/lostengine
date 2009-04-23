local Color = lost.common.Color
local Quad2D = lost.mesh.Quad2D
local Line2D = lost.mesh.Line2D
local Material = lost.mesh.Material
local Vec2 = lost.math.Vec2


-- requires camera3D to be set from C++
function init(loader)
  local shader = lost.gl.loadShader(loader, "light")
  shader:enable()
  shader:validate()
  if not shader:validated() then
    log.debug("Problem found during validation: \n"..shader:log())
  else
    log.debug("Program validated OK")
  end
  shader:set("LightPosition", camera3D:position());
  shader:set("LightDiffuse",  Color(1.0, 1.0, 1.0));
  shader:set("LightAmbient",  Color(1.0, 1.0, 1.0));
  shader:set("LightSpecular", Color(1.0, 1.0, 1.0));  
  shader:disable()
  
  -- these are deliberately global, so the C++ part can fetch the easier from the interpreter
  quad = Quad2D.create(loader:load("zim.png"), true)
  line = Line2D.create(Vec2(0,0), Vec2(400,400))
  line.material.color = Color(1,1,0)
  log.debug("loading file")
  filedata = loader:load("cube_tri.obj")
  log.debug("parsing file")
  cube = lost.mesh.Loader.obj(filedata);
  cube.material.color = Color(0,1,0)
  cube.material.shader = shader
  log.debug("init ok")
end
