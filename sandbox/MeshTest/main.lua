local Color = lost.common.Color

-- requires camera3D to be set from C++
function initShaders(loader)
  local shader = lost.gl.loadShader(loader, "light")
  shader:enable()
  shader:validate()
  if not shader:validated() then
    log.debug("Problem found during validation: \n"..lightShader:log())
  else
    log.debug("Program validated OK")
  end
  shader:set("LightPosition", camera3D:position());
  shader:set("LightDiffuse",  Color(1.0, 0.0, 0.0));
  shader:set("LightAmbient",  Color(1.0, 0.0, 0.0));
  shader:set("LightSpecular", Color(1.0, 0.0, 0.0));  
  shader:disable()
  return shader
end
